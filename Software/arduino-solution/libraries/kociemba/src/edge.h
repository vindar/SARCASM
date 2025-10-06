#pragma once

#include "arduino_undefine.h"

namespace kociemba
{


    // The names of the edge positions of the cube. Edge UR e.g., has an U(p) and R(ight) facelet.
    typedef enum {
        UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR
    } edge_t;

#define EDGE_COUNT 12

}
/* end of file */
