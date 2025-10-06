#include "coordcube.h"
#include "cubiecube.h"

#include "arduino_undefine.h"

namespace kociemba
{

    FLASHMEM void get_coordcube(cubiecube_t* cubiecube, coordcube_t* result)
    {
        result->twist = getTwist(cubiecube);
        result->flip = getFlip(cubiecube);
        result->parity = cornerParity(cubiecube);
        result->FRtoBR = getFRtoBR(cubiecube);
        result->URFtoDLF = getURFtoDLF(cubiecube);
        result->URtoUL = getURtoUL(cubiecube);
        result->UBtoDF = getUBtoDF(cubiecube);
        result->URtoDF = getURtoDF(cubiecube);// only needed in phase2
    }

}
/* end of file */


