#pragma once

#include "arduino_undefine.h"

namespace kociemba
{

    // The names of the corner positions of the cube. Corner URF e.g., has an U(p), a R(ight) and a F(ront) facelet
    typedef enum {
        URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB
    } corner_t;

#define CORNER_COUNT 8

}
/* end of file */
