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
// along with this code. If not, see <http://www.gnu.org/licenses/>.#pragma once

#include "cubeOrientation.h"
#include "misc.h"
#include "debug.h"
#include "panic.h"


namespace CubeOrientation
    {


    
    CubeOrientation cube;


    CubeOrientation::CubeOrientation()
        {
        reset();
        }



    void CubeOrientation::reset(char up, char right, char front, char down, char left, char back)
        {
        _cpos[0] = up;
        _cpos[1] = right;
        _cpos[2] = front;
        _cpos[3] = down;
        _cpos[4] = left;
        _cpos[5] = back;
        }



    int CubeOrientation::whereis(char face)
        {
        face = misc::uppercase(face);
        for (int i = 0; i < 6; i++)
            {
            if (_cpos[i] == face) return i;
            }
        ABORT("CubeOrientation::whereis() error");
        return 0;
        }


    /** compute a face rotation */
    SLOWFUN int CubeOrientation::get_rot(char facelook, char faceup)
        {
        switch (facelook)
            {
            case 'U':
                {
                switch (faceup)
                    {
                    case 'B': { return 0; }
                    case 'L': { return 1; }
                    case 'F': { return 2; }
                    case 'R': { return 3; }
                    }
                ABORT("CubeOrientation::get_rot() error 1");
                }

            case 'R':
                {
                switch (faceup)
                    {
                    case 'U': { return 0; }
                    case 'F': { return 1; }
                    case 'D': { return 2; }
                    case 'B': { return 3; }
                    }
                ABORT("CubeOrientation::get_rot() error 2");
                }

            case 'F':
                {
                switch (faceup)
                    {
                    case 'U': { return 0; }
                    case 'L': { return 1; }
                    case 'D': { return 2; }
                    case 'R': { return 3; }
                    }
                ABORT("CubeOrientation::get_rot() error 3");
                }

            case 'D':
                {
                switch (faceup)
                    {
                    case 'F': { return 0; }
                    case 'L': { return 1; }
                    case 'B': { return 2; }
                    case 'R': { return 3; }
                    }
                ABORT("CubeOrientation::get_rot() error 4");
                }

            case 'L':
                {
                switch (faceup)
                    {
                    case 'U': { return 0; }
                    case 'B': { return 1; }
                    case 'D': { return 2; }
                    case 'F': { return 3; }
                    }
                ABORT("CubeOrientation::get_rot() error 5");
                }

            case 'B':
                {
                switch (faceup)
                    {
                    case 'U': { return 0; }
                    case 'R': { return 1; }
                    case 'D': { return 2; }
                    case 'L': { return 3; }
                    }
                ABORT("CubeOrientation::get_rot() error 6");
                }
            }
        ABORT("CubeOrientation::get_rot() error 7");
        return -1;
        }


    int CubeOrientation::front_rotation()
        {
        const char facelook = whois_front();
        const char faceup = whois_up();
        return get_rot(facelook, faceup);
        }


    int CubeOrientation::back_rotation()
        {
        const char facelook = whois_back();
        const char faceup = whois_up();
        return get_rot(facelook, faceup);
        }


    int CubeOrientation::up_rotation()
        {
        const char facelook = whois_up();
        const char faceup = whois_back();
        return get_rot(facelook, faceup);
        }


    int CubeOrientation::down_rotation()
        {
        const char facelook = whois_down();
        const char faceup = whois_front();
        return get_rot(facelook, faceup);
        }


    int CubeOrientation::left_rotation()
        {
        const char facelook = whois_left();
        const char faceup = whois_up();
        return get_rot(facelook, faceup);
        }


    int CubeOrientation::right_rotation()
        {
        const char facelook = whois_right();
        const char faceup = whois_up();
        return get_rot(facelook, faceup);
        }


    void CubeOrientation::turn_clockwise()
        {
        char a = _cpos[1];
        _cpos[1] = _cpos[5];
        _cpos[5] = _cpos[4];
        _cpos[4] = _cpos[2];
        _cpos[2] = a;
        }


    void CubeOrientation::turn_anticlockwise()
        {
        char a = _cpos[1];
        _cpos[1] = _cpos[2];
        _cpos[2] = _cpos[4];
        _cpos[4] = _cpos[5];
        _cpos[5] = a;
        }


    void CubeOrientation::turn_half()
        {
        turn_clockwise();
        turn_clockwise();
        }


    void CubeOrientation::tumble()
        {
        char a = _cpos[0];
        _cpos[0] = _cpos[5];
        _cpos[5] = _cpos[3];
        _cpos[3] = _cpos[2];
        _cpos[2] = a;
        }


    void CubeOrientation::make_move(char c)
        {
        c = misc::uppercase(c);
        switch (c)
            {
            case 'T': { tumble(); break; }
            case 'R': { turn_clockwise(); break; }
            case 'V': { turn_anticlockwise(); break; }
            case 'A': { turn_half(); break; }
            default: ABORT("CubeOrientation::make_move() incorrect move");
            }
        }


    void CubeOrientation::make_moves(const char* moves, int len)
        {
        while ((len != 0) && (moves[0] != 0))
            {
            make_move(moves[0]);
            len--; moves++;
            }
        }


    const char* CubeOrientation::find_move_for_top(char face)
        {
        const int k = whereis(face);
        switch (k)
            {
            case 0: { return ""; }
            case 1: { return "VT"; }
            case 2: { return "AT"; }
            case 3: { return "TT"; }
            case 4: { return "RT"; }
            case 5: { return "T"; }
            }
        DEBUG("CubeOrientation::find_move_for_top() error");
        return nullptr;
        }




}



//extern CubeOrientation CubeO;  // declaration of global cube orientation object

//CubeOrientation CubeO;		// global cube orientation object




/* end of file */ 

