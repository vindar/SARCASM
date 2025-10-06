#pragma once


#include "cache_arrays.h"

#include "cubiecube.h"

#include "arduino_undefine.h"


namespace kociemba
{

    typedef struct
    {
        // All coordinates are 0 for a solved cube except for UBtoDF, which is 114
        short twist;
        short flip;
        short parity;
        short FRtoBR;
        short URFtoDLF;
        short URtoUL;
        short UBtoDF;
        int URtoDF;
    } coordcube_t;


    void get_coordcube(cubiecube_t* cubiecube, coordcube_t* result);

}
/* end of file */


