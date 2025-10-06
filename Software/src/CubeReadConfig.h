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
// along with this code. If not, see  <http://www.gnu.org/licenses/>.

#pragma once

#include <tgx.h>
#include <array>


namespace CubeRead
    {

    /* light color and intensity when reading the cube. */
    const int cube_read_minled = 2;  // the first led to light
    const int cube_read_maxled = 22; // the last led to light
    const int cube_read_R = 50;      // red value 
    const int cube_read_G = 50;      // green value
    const int cube_read_B = 50;      // blue value
    const int cube_read_W = 20;      // white value


    
    /** the rectangle for red color calibration */
    //const tgx::iBox2 R_red(96, 124, 291, 354);

    /** the rectangle for white color calibration */
    //const tgx::iBox2 R_white(12, 26, 280, 364);

    /** the rectangle for black color calibration */
    //const tgx::iBox2 R_black(163, 177, 287, 357);

    /** the rectangle 'small' facets for color identification. order:
     * 0 1 2
     * 3 4 5
     * 6 7 8
     **/
    std::array<tgx::iBox2, 9> cR = {
    tgx::iBox2(341,365,436,456),
    tgx::iBox2(334,362,311,336),
    tgx::iBox2(331,355,176,200),
    tgx::iBox2(446,467,419,439),
    tgx::iBox2(447,475,302,335),
    tgx::iBox2(448,471,197,220),
    tgx::iBox2(534,581,403,423),
    tgx::iBox2(547,565,295,341),
    tgx::iBox2(540,583,217,235)
        };

    std::array<tgx::iBox2, 9> cRbig = {
    tgx::iBox2(289,404,392,504),
    tgx::iBox2(292,411,263,379),
    tgx::iBox2(285,407,133,245),
    tgx::iBox2(414,502,389,483),
    tgx::iBox2(418,509,263,375),
    tgx::iBox2(418,509,147,248),
    tgx::iBox2(516,600,381,455),
    tgx::iBox2(520,593,270,368),
    tgx::iBox2(520,600,183,253)
        };



    /** Luminosity correction coefficients for facet 3 */

    struct LumCoeffs { tgx::fVec3 a, b, c, d; };

    const LumCoeffs luminosity_corr[9] = {
    LumCoeffs{ tgx::fVec3(4.481795e-01f, 5.389177e-01f, 4.151965e-01f),	 tgx::fVec3(2.830570e-01f, 2.692731e-01f, 2.512286e-01f),	 tgx::fVec3(6.784348e-02f, 7.904839e-02f, 5.459216e-02f),	 tgx::fVec3(-1.043957e-01f, -1.459904e-01f, -7.290971e-02f) },
    LumCoeffs{ tgx::fVec3(2.128570e-01f, 4.439844e-01f, 2.258195e-01f),	 tgx::fVec3(3.766758e-01f, 2.391659e-01f, 3.004127e-01f),	 tgx::fVec3(1.381074e-01f, 8.436226e-02f, 8.089458e-02f),	 tgx::fVec3(-1.788133e-01f, -1.422752e-01f, -9.134544e-02f) },
    LumCoeffs{ tgx::fVec3(3.208661e-01f, 5.198386e-01f, 3.460231e-01f),	 tgx::fVec3(3.518463e-01f, 2.558486e-01f, 2.845695e-01f),	 tgx::fVec3(1.090985e-01f, 6.760378e-02f, 6.494300e-02f),	 tgx::fVec3(-1.603708e-01f, -1.467143e-01f, -7.954784e-02f) },
    LumCoeffs{ tgx::fVec3(1.000000e+00f, 1.000000e+00f, 1.000000e+00f),	 tgx::fVec3(0.000000e+00f, 0.000000e+00f, 0.000000e+00f),	 tgx::fVec3(0.000000e+00f, 0.000000e+00f, 0.000000e+00f),	 tgx::fVec3(0.000000e+00f, 0.000000e+00f, 0.000000e+00f) },
    LumCoeffs{ tgx::fVec3(8.369502e-01f, 9.258916e-01f, 7.849625e-01f),	 tgx::fVec3(-1.121703e-01f, -3.720752e-02f, 2.273834e-01f),	 tgx::fVec3(-4.449248e-02f, -4.998065e-03f, 2.233745e-02f),	 tgx::fVec3(1.251568e-01f, 3.131444e-02f, -1.474879e-02f) },
    LumCoeffs{ tgx::fVec3(8.485349e-01f, 9.332386e-01f, 9.466497e-01f),	 tgx::fVec3(1.393719e-01f, 5.119227e-02f, 4.468207e-02f),	 tgx::fVec3(1.592177e-02f, 4.788072e-03f, 1.337088e-03f),	 tgx::fVec3(-4.778456e-02f, -2.462141e-02f, -1.093372e-03f) },
    LumCoeffs{ tgx::fVec3(3.269686e+00f, 3.149203e+00f, 2.739627e+00f),	 tgx::fVec3(-3.236956e+00f, -2.782824e+00f, -9.593554e-01f),	 tgx::fVec3(-1.599447e-01f, -1.985607e-01f, -7.126978e-02f),	 tgx::fVec3(5.252852e-01f, 5.338626e-01f, 1.411379e-01f) },
    LumCoeffs{ tgx::fVec3(2.170692e+00f, 2.168281e+00f, 1.874778e+00f),	 tgx::fVec3(-1.439610e+00f, -1.357592e+00f, -8.428667e-02f),	 tgx::fVec3(-1.118010e-01f, -1.122282e-01f, -2.296588e-02f),	 tgx::fVec3(3.313806e-01f, 3.130021e-01f, 3.820278e-02f) },
    LumCoeffs{ tgx::fVec3(2.923078e+00f, 2.921026e+00f, 2.547752e+00f),	 tgx::fVec3(-2.538887e+00f, -2.406411e+00f, -5.823638e-01f),	 tgx::fVec3(-1.656587e-01f, -2.031715e-01f, -6.655087e-02f),	 tgx::fVec3(4.579461e-01f, 4.959000e-01f, 1.193475e-01f) } };

 
    }


/** end of file */
