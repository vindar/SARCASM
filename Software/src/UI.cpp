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


#include "config.h"
#include "UI.h"
#include "charging_icon.h"
#include "panic.h"
#include "faceAction.h"

#include <tgx.h>
#include "font_SourceCodePro_AA4.h"
#include "font_SourceCodePro_Bold_AA4.h"
#include <Arduino.h>



#define UI_CMD_X 7
#define UI_CMD_Y 5

#define UI_BOLT_X 242
#define UI_BOLT_Y 2
#define UI_BOLT_W 10
#define UI_BOLT_H 16

#define UI_VOLT_X 266
#define UI_VOLT_Y 5
#define UI_VOLT_W 10
#define UI_VOLT_H 14

#define UI_VOLT_MA 0.99f
#define UI_VOLT_MINCHANGE 10
#define UI_VOLT_TIME_CHANGE 1500

#define UI_ACT_X 125
#define UI_ACT_Y 4
#define UI_ACT_W 11
#define UI_ACT_H 11
#define UI_ACT_NB 9


namespace UI
    {

    const tgx::RGB565 colorUI = tgx::RGB32(65, 135, 113);
    const tgx::RGB565 colorUI_dimmed = tgx::RGB32(45, 95, 79);
    const tgx::RGB565 colorUI_activity = tgx::RGB32(45, 95, 79);

    

    SLOWFUN void drawMainUI(tgx::Image<tgx::RGB565>& im)
        {       
        faceAction::drawFace(im);
        im.drawTextEx("SARCASM CMD", { UI_CMD_X, UI_CMD_Y }, font_SourceCodePro_Bold_AA4_16, tgx::Anchor::TOPLEFT, false, false, colorUI, 1.0f); // COMMAND TEXT
        im.drawThickRoundRectAA(tgx::fBox2(3, 317, 20, 237), 9, 3, colorUI, 1.0f); // border
        }



    void drawUIvoltage(tgx::Image<tgx::RGB565>& im, float voltage, float current)
        {
        static elapsedMillis emchanged = 0; 
        static float _lastVoltage = -1.0f; // last voltage value
        static int _prevVoltage100 = -1; // last displayed voltage value (x100, i.e. 7.45V = 745)

        // draw the charging icon. 
        int newBolt = (current > CHARGING_CURRENT_FAST) ? 2 : ((current > CHARGING_CURRENT_SLOW) ? 1 : 0); // 0 = no charging, 1 = slow charging, 2 = fast charging
        if (newBolt >= 2) im.blitMasked(charging_icon, tgx::RGB565(0, 0, 0), { UI_BOLT_X, UI_BOLT_Y }, 1.0f);
        if (newBolt >= 1) im.blitMasked(charging_icon, tgx::RGB565(0, 0, 0), { UI_BOLT_X + UI_BOLT_W, UI_BOLT_Y }, 1.0f);
        // display current battery voltage
        
        _lastVoltage = (_lastVoltage < 0.0f) ? voltage : (_lastVoltage * UI_VOLT_MA + voltage * (1.0f - UI_VOLT_MA)); // apply a low-pass filter to the voltage value
        const int voltage100 = (int)(_lastVoltage * 100);

        const int dd = abs(voltage100 - _prevVoltage100);
        if ((dd > UI_VOLT_MINCHANGE) || ((dd > 2) && (emchanged > UI_VOLT_TIME_CHANGE)))
            {
            _prevVoltage100 = voltage100; 
            emchanged = 0;
            }
        char vt[6];
        vt[0] = (_prevVoltage100 / 100) + '0';
        vt[1] = '.';
        vt[2] = ((_prevVoltage100 / 10) % 10) + '0';
        vt[3] = ((_prevVoltage100 % 10)) + '0';
        vt[4] = 'V';
        vt[5] = 0;
        im.drawTextEx(vt, { UI_VOLT_X, UI_VOLT_Y }, font_SourceCodePro_AA4_16, tgx::Anchor::TOPLEFT, false, false, colorUI, 1.0f);
        }



    void drawUIactivity(tgx::Image<tgx::RGB565>& im, tgx::iVec2 act)
        {
        act.x = max(0, act.x);
        act.y = min(8, act.y);
        im.drawRect({ UI_ACT_X, UI_ACT_X + UI_ACT_NB * (UI_ACT_W + 1), UI_ACT_Y, UI_ACT_Y + UI_ACT_H + 1 }, colorUI);
        for (int k = 1; k < UI_ACT_NB; k++)
            {
            im.drawFastVLine({ UI_ACT_X + k * (UI_ACT_W + 1) , UI_ACT_Y }, UI_ACT_H + 1, colorUI);
            }
        for (int k = 0; k < UI_ACT_NB; k++)
            {
            if ((k >= act.x) && (k <= act.y))
                { // draw the activity bar
                im.fillRect({ UI_ACT_X + k * (UI_ACT_W + 1) + 1, UI_ACT_X + (k + 1) * (UI_ACT_W + 1) - 1, UI_ACT_Y + 1 , UI_ACT_Y + UI_ACT_H }, colorUI_activity);
                }
            }
        }
    }


/** end of file */


