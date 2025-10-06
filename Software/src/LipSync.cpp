//
// Copyright 2021 Arvind Singh
//
// This file is part of the S.A.R.C.A.S.M. source code.
//
// This is a free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this code. If not, see <http://www.gnu.org/licenses/>.
//

#include "config.h"
#include "LipSync.h"
#include "debug.h"

#include <Arduino.h>
#include <Audio.h>
#include <teensythreads.h>

namespace LipSync
    {



    #define FEAT_ATTACK 0x01
    #define FEAT_CLIP   0x02

    #define LIPSYNC_PUB_PERIOD_MS 20 


    // shared data between ISR and main code
    struct LipsyncFeat {
        uint32_t t_ms;     // millis() 
        uint16_t env_q15;  // enveloppe 0..32767 (Q15)
        uint16_t zcr_q15;  // zero-cross rate 0..32767 (Q15)
        uint8_t  flags;    // FEAT_*
        uint8_t  _pad[3];  // alignement
        };

    volatile LipsyncFeat g_feat_last;   // writen by isr, read by main code
    volatile uint32_t    g_gen = 0;     // compteur de génération (incrémenté UNE fois par publication)

    static uint16_t s_env_q15 = 0; // Envelop (Q15). Choix "classique": attaque rapide, release lent.    
    static const uint16_t ATT_Q15 = 19660; // ATT = 0.60 -> Q15(0.60) = 19660    
    static const uint16_t REL_Q15 = 983; // REL = 0.03 -> Q15(0.03) = 983 (descente lente)    
    static const uint16_t ATTACK_THR_Q15 = 1638; // attack threshold (peak - env_prev) = 0.05 -> Q15(0.05) = 1638    
    static const int16_t  CLIP_LIM = 32000;// clip detection limit

    // gain to peak before envelop update.
    // 1.0 = 32767 ; 0.5 = 16384 ; 2.0 = 65534 (sera saturé à 1.0 plus loin)
    volatile uint16_t g_lipsync_gain_q15 = 32767; // par défaut 1.0

    static inline void set_lipsync_gain_q15(uint16_t g) 
        {
        g_lipsync_gain_q15 = g;
        }

    // aggregation
    static uint32_t s_pub_last_ms = 0;   // t0 de la fenêtre courante
    static uint32_t s_acc_env_sum = 0;   // somme des env (Q15)
    static uint16_t s_acc_env_max = 0;   // max des env (Q15)
    static uint32_t s_acc_zcr_sum = 0;   // somme des ZCR (Q15)
    static uint16_t s_acc_count = 0;   // nombre de blocs agrégés
    static uint8_t  s_acc_flags = 0;   // OR des flags (ATTACK/CLIP)

    
    static inline int16_t iabs16(int16_t x) { return (x < 0) ? (int16_t)-x : x; }

    static inline uint16_t lipsync_compute_zcr_q15(const int16_t* s, int N)
        {
        if (!s || N <= 1) return 0;
        int cnt = 0;
        int16_t prev = s[0];
        for (int i = 1; i < N; ++i) 
            {
            int16_t v = s[i];
            if ((prev ^ v) < 0) cnt++;  // crossing
            prev = v;
            }
        uint32_t q = (uint32_t)cnt * 32767u / (uint32_t)(N - 1);
        return (uint16_t)q;
        }

    
    static inline void lipsync_isr_publish(uint32_t t_ms, uint16_t env_q15, uint16_t zcr_q15, uint8_t  flags)
        {
        g_feat_last.t_ms = t_ms;
        g_feat_last.env_q15 = env_q15;
        g_feat_last.zcr_q15 = zcr_q15;
        g_feat_last.flags = flags;
        __DMB();   // prevent reordering
        g_gen++;   // publish
        }

    
    /**   
    * should be called for each audio bloc
    * set sample to nullptr if no audio
    **/
    void lipsync_isr_process_block(const int16_t* samples, int N)
        {
        // Peak + clipping
        int16_t peak_i = 0;
        bool clipped = false;
        if (samples && N > 0) 
            {
            for (int i = 0; i < N; ++i) 
                {
                int16_t a = iabs16(samples[i]);
                if (a > peak_i) peak_i = a;
                if (!clipped && (samples[i] >= CLIP_LIM || samples[i] <= -CLIP_LIM)) { clipped = true; }
                }
            }

        // normalized peak Q15, gain and saturation
        uint16_t peak_q15 = (uint16_t)((peak_i > 32767) ? 32767 : peak_i);
        uint32_t scaled = ((uint32_t)peak_q15 * (uint32_t)g_lipsync_gain_q15) >> 15;
        if (scaled > 32767u) scaled = 32767u;
        peak_q15 = (uint16_t)scaled;

        // envelop update (attack/release) Q15
        uint16_t env_prev = s_env_q15;
        uint16_t env = env_prev;
        if (peak_q15 > env) 
            {
            uint16_t d = (uint16_t)(peak_q15 - env);
            env = (uint16_t)(env + ((uint32_t)ATT_Q15 * d >> 15));
            } 
        else 
            {
            uint16_t d = (uint16_t)(env - peak_q15);
            env = (uint16_t)(env - ((uint32_t)REL_Q15 * d >> 15));
            }
        s_env_q15 = env;

        // Flags 
        uint8_t flags = 0;
        if ((int32_t)peak_q15 - (int32_t)env_prev > (int32_t)ATTACK_THR_Q15) flags |= FEAT_ATTACK;
        if (clipped) flags |= FEAT_CLIP;

        // ZCR on bloc
        uint16_t zcr_q15 = lipsync_compute_zcr_q15(samples, N);

        // aggregation
        if (s_acc_count == 0 && s_pub_last_ms == 0) { s_pub_last_ms = millis(); }
        s_acc_env_sum += env;
        if (env > s_acc_env_max) s_acc_env_max = env;
        s_acc_zcr_sum += zcr_q15;
        s_acc_flags |= flags;
        if (s_acc_count < 0xFFFF) s_acc_count++;

        // publish measurement
        uint32_t now_ms = millis();
        if ((now_ms - s_pub_last_ms) >= (uint32_t)LIPSYNC_PUB_PERIOD_MS) 
            {
            // représentatif: mix 75% max + 25% moyenne (capte bien les syllabes)
            uint16_t mean_env = (s_acc_count ? (uint16_t)(s_acc_env_sum / s_acc_count) : 0);
            uint16_t env_pub = (uint16_t)(((uint32_t)s_acc_env_max * 3 + mean_env) >> 2);
            uint16_t zcr_pub = (s_acc_count ? (uint16_t)(s_acc_zcr_sum / s_acc_count) : 0);
            uint8_t  fl_pub = s_acc_flags;

            lipsync_isr_publish(now_ms, env_pub, zcr_pub, fl_pub);

            // reset fenetre
            s_pub_last_ms = now_ms;
            s_acc_env_sum = 0;
            s_acc_env_max = 0;
            s_acc_zcr_sum = 0;
            s_acc_count = 0;
            s_acc_flags = 0;
            }
        }









    SLOWFUN static inline bool lipsync_read_latest(LipsyncFeat& out)
        {
        for (int tries = 0; tries < 3; ++tries) 
            {
            uint32_t g1 = g_gen;
            __DMB();
            LipsyncFeat tmp;
            tmp.t_ms = g_feat_last.t_ms;
            tmp.env_q15 = g_feat_last.env_q15;
            tmp.zcr_q15 = g_feat_last.zcr_q15;
            tmp.flags = g_feat_last.flags;
            __DMB();
            uint32_t g2 = g_gen;
            if (g1 == g2) { out = tmp; return true; }
            }
        return false; 
        }





    // Neutral rapide quand c'est calme (fin de phrase)
    static const uint16_t QUIET_TO_NEUTRAL_Q15 = (uint16_t)(0.06f * 32767); // 6%
    static const uint32_t QUIET_NEUTRAL_MS = 90;                        // 90 ms

    // Seuils d'hystérésis en Q15 (1.0 = 32767)
    static const uint16_t T_NEUTRAL_UP = (uint16_t)(0.12f * 32767); // 3932
    static const uint16_t T_NEUTRAL_DOWN = (uint16_t)(0.08f * 32767); // 2621
    static const uint16_t T_OPEN_UP = (uint16_t)(0.35f * 32767); // 11468
    static const uint16_t T_OPEN_DOWN = (uint16_t)(0.25f * 32767); // 8192

    // Durées minimales par état (évite papillotement)
    static const uint32_t HOLD_CLOSE_MS = 40; // 50
    static const uint32_t HOLD_NEUT_MS = 50; // 60
    static const uint32_t HOLD_OPEN_MS = 70; // 90
   


    // Lissage coté affichage (plus doux que l’ISR)
    static const uint16_t DISP_ATT_Q15 = (uint16_t)(0.55f * 32767); // 18021
    static const uint16_t DISP_REL_Q15 = (uint16_t)(0.04f * 32767); // 1311

    // Boost bref sur attaque (force ouverture courte)
    static const uint32_t ATTACK_BOOST_MS = 40;

    // Optionnel : usage simple de ZCR pour stabiliser les voyelles
    static const uint16_t ZCR_LOW_Q15 = (uint16_t)(0.25f * 32767); // 8192
    static const uint16_t ZCR_HIGH_Q15 = (uint16_t)(0.50f * 32767); // 16384



    static const uint32_t SPEAKING_RECENT_MS = 120;   // fenêtre "parle-t-on ?" en ms


    // Parle-t-on "récemment" ? (utilise la dernière mesure publiée par l'ISR)
    SLOWFUN bool speaking_recent()
        {
        uint32_t now_ms = millis();
        LipsyncFeat f;
        if (!lipsync_read_latest(f)) return false;
        if (now_ms - f.t_ms > SPEAKING_RECENT_MS) return false;
        return (f.env_q15 > T_NEUTRAL_UP) || (f.flags & FEAT_ATTACK);
        }

    // -------------------- État interne du décideur --------------------
    static uint16_t   s_env_disp_q15 = 0;           // enveloppe lissée affichage
    static MouthState s_state = M_NEUTRAL;          // défaut quand machine muette
    static uint32_t   s_state_since_ms = 0;
    static uint32_t   s_attack_hold_until_ms = 0;
    static uint32_t   s_last_seen_ms = 0;           // dernière publication vue
    static uint32_t   s_last_speech_ms = 0;  // dernière activité de parole détectée








    /** Select the mouth state */
    SLOWFUN MouthState mouthState()
        {
        uint32_t now_ms = millis();
        LipsyncFeat f;
        if (lipsync_read_latest(f)) 
            {
            s_last_seen_ms = f.t_ms;

            // Lissage affichage (Q15)
            uint16_t env = s_env_disp_q15;
            if (f.env_q15 > env) 
                {
                uint16_t d = (uint16_t)(f.env_q15 - env);
                env = (uint16_t)(env + ((uint32_t)DISP_ATT_Q15 * d >> 15));
                } 
            else 
                {
                uint16_t d = (uint16_t)(env - f.env_q15);
                env = (uint16_t)(env - ((uint32_t)DISP_REL_Q15 * d >> 15));
                }
            s_env_disp_q15 = env;

            // activité de parole
            if (f.env_q15 > T_NEUTRAL_UP || (f.flags & FEAT_ATTACK)) 
                {
                s_last_speech_ms = now_ms;
                }

            // neutral rapide si calme prolongé
            if (s_env_disp_q15 < QUIET_TO_NEUTRAL_Q15 && (now_ms - s_last_speech_ms) > QUIET_NEUTRAL_MS) 
                {
                if (s_state != M_NEUTRAL) 
                    {
                    s_state = M_NEUTRAL;
                    s_state_since_ms = now_ms;
                    }
                s_attack_hold_until_ms = 0;
                s_env_disp_q15 = 0;
                return s_state;
                }


            // 2) Boost attaque: courte ouverture "syllabe"
            if (f.flags & FEAT_ATTACK) 
                {
                s_attack_hold_until_ms = now_ms + ATTACK_BOOST_MS;
                }

            // 3) Ajustements légers via ZCR (facultatif, cheap)
            uint16_t T_open_up = T_OPEN_UP;
            uint16_t T_open_down = T_OPEN_DOWN;
            if (f.zcr_q15 < ZCR_LOW_Q15) 
                {
                // voyelles (voisé) : tenir l'ouverture un peu plus
                uint16_t delta = (uint16_t)(T_OPEN_DOWN >> 4); // +6.25%
                T_open_down = (uint16_t)((T_open_down + delta) > 32767 ? 32767 : (T_open_down + delta));
                } 
            else if (f.zcr_q15 > ZCR_HIGH_Q15) 
                {
                // fricatives / bruit : ouvrir plus facilement (un poil)
                uint16_t delta = (uint16_t)(T_OPEN_UP >> 5);   // -3.125%
                T_open_up = (T_open_up > delta) ? (T_open_up - delta) : 0;
                }

            // 4) Hystérésis + min-hold
            uint32_t held_ms = now_ms - s_state_since_ms;
            MouthState st = s_state;

            switch (s_state) 
                {
                case M_OPEN:
                    if (held_ms >= HOLD_OPEN_MS) 
                        { 
                        if (env < T_open_down) st = M_NEUTRAL; 
                        }
                    break;

                case M_NEUTRAL:
                    if (env > T_open_up) 
                        { 
                        if (held_ms >= HOLD_NEUT_MS) st = M_OPEN; 
                        } 
                    else if (env < T_NEUTRAL_DOWN) 
                        {
                        if (held_ms >= HOLD_NEUT_MS) st = M_CLOSE;
                        }
                    break;

                case M_CLOSE:
                    if (held_ms >= HOLD_CLOSE_MS) 
                        {
                        if (env > T_NEUTRAL_UP) st = M_NEUTRAL;
                        }
                    break;
                }

                // 5) Petit coup de pouce si attaque très récente
                if (st != M_OPEN && now_ms < s_attack_hold_until_ms && held_ms >= 30) 
                    {
                    st = M_OPEN;
                    }

                if (st != s_state) 
                    {
                    s_state = st;
                    s_state_since_ms = now_ms;
                    }
            } 
        else 
            {
            // Pas de snapshot stable : conserve l'état courant
            // (optionnel) si plus de 250 ms sans données, revenir doucement vers NEUTRAL
            if (now_ms - s_last_seen_ms > 250 && s_state != M_NEUTRAL) 
                {
                s_state = M_NEUTRAL;
                s_state_since_ms = now_ms;
                s_env_disp_q15 = 0;
                }
            }
        return s_state;
        }

    }





/** end of file */