#include "cache_arrays.h"
#include "color.h"
#include "facecube.h"
#include "coordcube.h"

#include "arduino_undefine.h"


namespace kociemba
{


    typedef struct {
        int ax[31];             // The axis of the move
        int po[31];             // The power of the move
        int flip[31];           // phase1 coordinates
        int twist[31];
        int slice[31];
        int parity[31];         // phase2 coordinates
        int URFtoDLF[31];
        int FRtoBR[31];
        int URtoUL[31];
        int UBtoDF[31];
        int URtoDF[31];
        int minDistPhase1[31];  // IDA* distance do goal estimations
        int minDistPhase2[31];
    } search_t;



    FLASHMEM const char* solutionToString(search_t* search, int length, int depthPhase1)
    {
        static char s[3 * 32 + 5];
        int cur = 0;
        for (int i = 0; i < length; i++)
        {
            switch (search->ax[i])
            {
            case 0:
                s[cur++] = 'U';
                break;
            case 1:
                s[cur++] = 'R';
                break;
            case 2:
                s[cur++] = 'F';
                break;
            case 3:
                s[cur++] = 'D';
                break;
            case 4:
                s[cur++] = 'L';
                break;
            case 5:
                s[cur++] = 'B';
                break;
            }
            switch (search->po[i])
            {
            case 1:
                s[cur++] = ' ';
                break;
            case 2:
                s[cur++] = '2';
                s[cur++] = ' ';
                break;
            case 3:
                s[cur++] = '\'';
                s[cur++] = ' ';
                break;
            }
            if (i == depthPhase1 - 1)
            {
                s[cur++] = '.';
                s[cur++] = ' ';
            }
        }
        s[cur] = 0;
        return s;
    }



    // Apply phase2 of algorithm and return the combined phase1 and phase2 depth. In phase2, only the moves
    // U,D,R2,F2,L2 and B2 are allowed.
    FLASHMEM int totalDepth(search_t* search, int depthPhase1, int maxDepth)
    {
        int mv = 0, d1 = 0, d2 = 0, i;
        int maxDepthPhase2 = (10 < maxDepth - depthPhase1) ? 10 : (maxDepth - depthPhase1);// Allow only max 10 moves in phase2
        int depthPhase2;
        int n;
        int busy;

        for (i = 0; i < depthPhase1; i++)
        {
            mv = 3 * search->ax[i] + search->po[i] - 1;
            search->URFtoDLF[i + 1] = URFtoDLF_Move[search->URFtoDLF[i]][mv];
            search->FRtoBR[i + 1] = (search->FRtoBR[i] < 24) ? FRtoBR_Move24_2[search->FRtoBR[i]][mv] : FRtoBR_Move[search->FRtoBR[i]][mv];
            search->parity[i + 1] = parityMove[search->parity[i]][mv];
        }

        if ((d1 = getPruning(Slice_URFtoDLF_Parity_Prun,
            (N_SLICE2 * search->URFtoDLF[depthPhase1] + search->FRtoBR[depthPhase1]) * 2 + search->parity[depthPhase1])) > maxDepthPhase2)
            return -1;

        for (i = 0; i < depthPhase1; i++)
        {
            mv = 3 * search->ax[i] + search->po[i] - 1;
            search->URtoUL[i + 1] = UBtoDF_Move[search->URtoUL[i]][mv]; //  initially: search->URtoUL[i + 1] = URtoUL_Move[search->URtoUL[i]][mv]  but URtoUL_Move and UBtoDF_Move are the same tables
            search->UBtoDF[i + 1] = UBtoDF_Move[search->UBtoDF[i]][mv];
        }
        search->URtoDF[depthPhase1] = MergeURtoULandUBtoDF[search->URtoUL[depthPhase1]][search->UBtoDF[depthPhase1]];

        if ((d2 = getPruning(Slice_URtoDF_Parity_Prun,
            (N_SLICE2 * search->URtoDF[depthPhase1] + search->FRtoBR[depthPhase1]) * 2 + search->parity[depthPhase1])) > maxDepthPhase2)
            return -1;

        if ((search->minDistPhase2[depthPhase1] = (d1 > d2 ? d1 : d2)) == 0)// already solved
            return depthPhase1;

        // now set up search

        depthPhase2 = 1;
        n = depthPhase1;
        busy = 0;
        search->po[depthPhase1] = 0;
        search->ax[depthPhase1] = 0;
        search->minDistPhase2[n + 1] = 1;// else failure for depthPhase2=1, n=0
        // +++++++++++++++++++ end initialization +++++++++++++++++++++++++++++++++
        do {
            do {
                if ((depthPhase1 + depthPhase2 - n > search->minDistPhase2[n + 1]) && !busy) {

                    if (search->ax[n] == 0 || search->ax[n] == 3)// Initialize next move
                    {
                        search->ax[++n] = 1;
                        search->po[n] = 2;
                    }
                    else {
                        search->ax[++n] = 0;
                        search->po[n] = 1;
                    }
                }
                else if ((search->ax[n] == 0 || search->ax[n] == 3) ? (++search->po[n] > 3) : ((search->po[n] = search->po[n] + 2) > 3)) {
                    do {// increment axis
                        if (++search->ax[n] > 5) {
                            if (n == depthPhase1) {
                                if (depthPhase2 >= maxDepthPhase2)
                                    return -1;
                                else {
                                    depthPhase2++;
                                    search->ax[n] = 0;
                                    search->po[n] = 1;
                                    busy = 0;
                                    break;
                                }
                            }
                            else {
                                n--;
                                busy = 1;
                                break;
                            }

                        }
                        else {
                            if (search->ax[n] == 0 || search->ax[n] == 3)
                                search->po[n] = 1;
                            else
                                search->po[n] = 2;
                            busy = 0;
                        }
                    } while (n != depthPhase1 && (search->ax[n - 1] == search->ax[n] || search->ax[n - 1] - 3 == search->ax[n]));
                }
                else
                    busy = 0;
            } while (busy);
            // +++++++++++++ compute new coordinates and new minDist ++++++++++
            mv = 3 * search->ax[n] + search->po[n] - 1;

            search->URFtoDLF[n + 1] = URFtoDLF_Move[search->URFtoDLF[n]][mv];

            search->FRtoBR[n + 1] = (search->FRtoBR[n] < 24) ? FRtoBR_Move24_2[search->FRtoBR[n]][mv] : FRtoBR_Move[search->FRtoBR[n]][mv];
            search->parity[n + 1] = parityMove[search->parity[n]][mv];
            search->URtoDF[n + 1] = URtoDF_Move[search->URtoDF[n]][mv];

            const signed char aa = getPruning(Slice_URtoDF_Parity_Prun,
                (N_SLICE2 * search->URtoDF[n + 1] + search->FRtoBR[n + 1]) * 2 + search->parity[n + 1]);
            const signed char bb = getPruning(Slice_URFtoDLF_Parity_Prun,
                (N_SLICE2 * search->URFtoDLF[n + 1] + search->FRtoBR[n + 1]) * 2 + search->parity[n + 1]);
            search->minDistPhase2[n + 1] = (aa > bb ? aa : bb);

            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        } while (search->minDistPhase2[n + 1] != 0);
        return depthPhase1 + depthPhase2;
    }




    template<bool FAST_FLIP, bool  FAST_TWIST>
    FLASHMEM const char* solution_template(const char* facelets, int maxDepth, int timeOut, int useSeparator)
    {
        search_t loc_search;
        search_t* const search = &loc_search;
        memset(search, 0, sizeof(search_t));

        facecube_t fc_loc;
        facecube_t* const fc = &fc_loc;
        memset(fc, 0, sizeof(facecube_t));

        cubiecube_t cc_loc;
        cubiecube_t* const cc = &cc_loc;
        memset(cc, 0, sizeof(cubiecube_t));

        coordcube_t c_loc;
        coordcube_t* const c = &c_loc;
        memset(c, 0, sizeof(coordcube_t));

        int s, i;
        int mv, n;
        int busy;
        int depthPhase1;


        // +++++++++++++++++++++check for wrong input +++++++++++++++++++++++++++++
        int count[6] = { 0 };
        for (i = 0; i < 54; i++)
            switch (facelets[i]) {
            case 'U':
                count[U]++;
                break;
            case 'R':
                count[R]++;
                break;
            case 'F':
                count[F]++;
                break;
            case 'D':
                count[D]++;
                break;
            case 'L':
                count[L]++;
                break;
            case 'B':
                count[B]++;
                break;
            }

        for (i = 0; i < 6; i++)
            if (count[i] != 9) { return NULL; }

        get_facecube_fromstring(facelets, fc);
        toCubieCube(fc, cc);
        if ((s = verify(cc)) != 0) { return NULL; }

        // +++++++++++++++++++++++ initialization +++++++++++++++++++++++++++++++++
        get_coordcube(cc, c);

        search->po[0] = 0;
        search->ax[0] = 0;
        search->flip[0] = c->flip;
        search->twist[0] = c->twist;
        search->parity[0] = c->parity;
        search->slice[0] = c->FRtoBR / 24;
        search->URFtoDLF[0] = c->URFtoDLF;
        search->FRtoBR[0] = c->FRtoBR;
        search->URtoUL[0] = c->URtoUL;
        search->UBtoDF[0] = c->UBtoDF;

        search->minDistPhase1[1] = 1;// else failure for depth=1, n=0
        mv = 0;
        n = 0;
        busy = 0;
        depthPhase1 = 1;

#if defined(TEENSYDUINO) || defined(ESP32)
        unsigned long start_time = millis();
#else
        const std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
#endif

        signed char prt[32];
        signed char prf[32];
        prf[0] = getPruning(Slice_Flip_Prun, N_SLICE1 * search->flip[0] + search->slice[0]);   // init distance using the large arrays in progmem
        prt[0] = getPruning(Slice_Twist_Prun, N_SLICE1 * search->twist[0] + search->slice[0]); //



        // +++++++++++++++++++ Main loop ++++++++++++++++++++++++++++++++++++++++++
        do {
            do {
                if ((depthPhase1 - n > search->minDistPhase1[n + 1]) && !busy)
                {
                    if (search->ax[n] == 0 || search->ax[n] == 3)// Initialize next move
                        search->ax[++n] = 1;
                    else
                        search->ax[++n] = 0;
                    search->po[n] = 1;
                }
                else if (++search->po[n] > 3)
                {
                    do {// increment axis
                        if (++search->ax[n] > 5)
                        {
                            #if defined(TEENSYDUINO) || defined(ESP32)
                            unsigned long elm = millis() - start_time;
                            if (elm > (unsigned long)timeOut) return nullptr;
                            #else
                            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() > timeOut) return nullptr;
                            #endif
                            
                            if (n == 0)
                            {
                                if (depthPhase1 >= maxDepth) return NULL;
                                else
                                {
                                    depthPhase1++;
                                    search->ax[n] = 0;
                                    search->po[n] = 1;
                                    busy = 0;
                                    break;
                                }
                            }
                            else
                            {
                                n--;
                                busy = 1;
                                break;
                            }
                        }
                        else
                        {
                            search->po[n] = 1;
                            busy = 0;
                        }
                    } while (n != 0 && (search->ax[n - 1] == search->ax[n] || search->ax[n - 1] - 3 == search->ax[n]));
                }
                else busy = 0;
            } while (busy);

            // +++++++++++++ compute new coordinates and new minDistPhase1 ++++++++++
            // if minDistPhase1 =0, the H subgroup is reached

            mv = 3 * search->ax[n] + search->po[n] - 1;
            search->flip[n + 1] = flipMove[search->flip[n]][mv];
            search->twist[n + 1] = twistMove[search->twist[n]][mv];
            search->slice[n + 1] = FRtoBR_Move24[search->slice[n]][mv];
            search->minDistPhase1[n + 1] = getMaxPruningFlipTwist<FAST_FLIP, FAST_TWIST>(search->slice[n + 1], search->flip[n + 1], search->twist[n + 1], prf[n], prt[n], prf[n + 1], prt[n + 1]);

            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            if (search->minDistPhase1[n + 1] == 0 && n >= depthPhase1 - 5)
            {
                search->minDistPhase1[n + 1] = 10;// instead of 10 any value >5 is possible
                if (n == depthPhase1 - 1 && (s = totalDepth(search, depthPhase1, maxDepth)) >= 0)
                {
                    if (s == depthPhase1 || (search->ax[depthPhase1 - 1] != search->ax[depthPhase1] && search->ax[depthPhase1 - 1] != search->ax[depthPhase1] + 3))
                    {
                        return solutionToString(search, s, (useSeparator ? depthPhase1 : -1));
                    }
                }
            }
        } while (1);
    }




    FLASHMEM const char* solve(const char* facelets, int maxDepth, int timeOut, int useSeparator)
    {
        if (Slice_Flip_Prun_fast == nullptr)
        {
            if (Slice_Twist_Prun_fast == nullptr)
            {
                return solution_template<false, false>(facelets, maxDepth, timeOut, useSeparator);
            }
            else
            {
                return solution_template<false, true>(facelets, maxDepth, timeOut, useSeparator);
            }
        }
        else
        {
            if (Slice_Twist_Prun_fast == nullptr)
            {
                return solution_template<true, false>(facelets, maxDepth, timeOut, useSeparator);
            }
            else
            {
                return solution_template<true, true>(facelets, maxDepth, timeOut, useSeparator);
            }
        }
    }

}
/* end of file */

