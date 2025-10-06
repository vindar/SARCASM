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

#pragma once

#include <tgx.h>


namespace UI
    {

    extern const tgx::RGB565 colorUI;
    extern const tgx::RGB565 colorUI_dimmed;
    extern const tgx::RGB565 colorUI_activity;


    /**
    * Redraw the full ui (with the background image)
    **/
    void drawMainUI(tgx::Image<tgx::RGB565>& im);

    /**
    * draw the voltage part of the UI
    * (fast, only update if value modified)
    **/
    void drawUIvoltage(tgx::Image<tgx::RGB565>& im, float voltage, float current);

    /**
    * draw the 'activity bar' the UI.
    * (fast, only update if value modified)
    **/
    void drawUIactivity(tgx::Image<tgx::RGB565>& im, tgx::iVec2 act);

    }



/** end of file */



