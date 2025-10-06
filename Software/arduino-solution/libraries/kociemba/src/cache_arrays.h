#pragma once

#include "arduino_undefine.h"


namespace kociemba
{



    // Representation of the cube on the coordinate level
    #define N_TWIST     2187
    #define N_FLIP      2048   
    #define N_SLICE1    495
    #define N_SLICE2    24
    #define N_PARITY    2
    #define N_URFtoDLF  20160
    #define N_FRtoBR    11880
    #define N_URtoUL    1320
    #define N_UBtoDF    1320
    #define N_URtoDF    20160
    #define N_URFtoDLB  40320
    #define N_URtoBR    479001600
    #define N_MOVE      18



    typedef short(*short_bidim_N_MOVE)[N_MOVE];


    // ******************************************Phase 1 move tables*****************************************************

    // Move table for the twists of the corners
    // twist < 2187 in phase 2.
    // twist = 0 in phase 2.
    extern short_bidim_N_MOVE twistMove; // of size [N_TWIST][N_MOVE] 77k


    // Move table for the flips of the edges
    // flip < 2048 in phase 1
    // flip = 0 in phase 2.
    extern short_bidim_N_MOVE flipMove;	// of size [N_FLIP][N_MOVE] 72k


    // Parity of the corner permutation. This is the same as the parity for the edge permutation of a valid cube.
    // parity has values 0 and 1
    extern short parityMove[2][18];



    // ***********************************Phase 1 and 2 movetable********************************************************

    // Move table for the four UD-slice edges FR, FL, Bl and BR
    // FRtoBRMove < 11880 in phase 1
    // FRtoBRMove < 24 in phase 2
    // FRtoBRMove = 0 for solved cube
    extern const short FRtoBR_Move[N_FRtoBR][N_MOVE];
    extern short FRtoBR_Move24_2[24][N_MOVE];
    extern short_bidim_N_MOVE FRtoBR_Move24;						// of size [N_FRtoBR/24][N_MOVE] 18k


    // Move table for permutation of six corners. The positions of the DBL and DRB corners are determined by the parity.
    // URFtoDLF < 20160 in phase 1
    // URFtoDLF < 20160 in phase 2
    // URFtoDLF = 0 for solved cube.
    extern const short URFtoDLF_Move[N_URFtoDLF][N_MOVE];


    // Move table for the permutation of six U-face and D-face edges in phase2. The positions of the DL and DB edges are
    // determined by the parity.
    // URtoDF < 665280 in phase 1
    // URtoDF < 20160 in phase 2
    // URtoDF = 0 for solved cube.
    extern const short URtoDF_Move[N_URtoDF][N_MOVE];


    // **************************helper move tables to compute URtoDF for the beginning of phase2************************

    // Table to merge the coordinates of the UR,UF,UL and UB,DR,DF edges at the beginning of phase2
    extern const short MergeURtoULandUBtoDF[336][336];

    // Move table for the three edges UB,DR and DF in phase1.
    extern short_bidim_N_MOVE UBtoDF_Move;                         // of size [N_UBtoDF][N_MOVE] 47k

    // Move table for the three edges UR,UF and UL in phase1.
    //extern short URtoUL_Move[N_URtoUL][N_MOVE];  // not used because it is the same as UBtoDF_Move



    // ****************************************Pruning tables for the search*********************************************

    // Pruning table for the permutation of the corners and the UD-slice edges in phase2.
    // The pruning table entries give a lower estimation for the number of moves to reach the solved cube.
    extern const signed char Slice_URFtoDLF_Parity_Prun[N_SLICE2 * N_URFtoDLF * N_PARITY / 2]; // in external memory


    // Pruning table for the permutation of the edges in phase2.
    // The pruning table entries give a lower estimation for the number of moves to reach the solved cube.
    extern const signed char Slice_URtoDF_Parity_Prun[N_SLICE2 * N_URtoDF * N_PARITY / 2];    // in external memory


    // Pruning table for the flip of the edges and the position (not permutation) of the UD-slice edges in phase1
    // The pruning table entries give a lower estimation for the number of moves to reach the H-subgroup.
    extern const signed char Slice_Flip_Prun[N_SLICE1 * N_FLIP / 2];          // 
    extern unsigned char* Slice_Flip_Prun_fast;					              // of size [N_SLICE1 * N_FLIP / 4] 248k -> dynamic allocation in local memory !


    // Pruning table for the twist of the corners and the position (not permutation) of the UD-slice edges in phase1
    // The pruning table entries give a lower estimation for the number of moves to reach the H-subgroup.
    extern const signed char Slice_Twist_Prun[N_SLICE1 * N_TWIST / 2 + 1];    // 
    extern unsigned char* Slice_Twist_Prun_fast;					          // of size [N_SLICE1 * N_TWIST / 4 + 1] 265k -> DMA MEMORY



    // Extract pruning value
    inline signed char getPruning(const signed char* table, int index)
    {
        return ((index & 1) == 0) ? (table[index >> 1] & 0x0f) : ((table[index >> 1] >> 4) & 0x0f);
    }



    template<bool FAST_FLIP, bool  FAST_TWIST>
    inline signed char getMaxPruningFlipTwist(int slice, int flip, int twist, signed char pr_f, signed char pr_t, signed char& newpr_f, signed char& newpr_t)
    {
        if (FAST_FLIP) // optimized away at compile time
        { // FLIP
            const int k = N_SLICE1 * flip + slice;
            const int i = k / 4;
            const int mo = 2 * (k & 3);
            int d = (int)((Slice_Flip_Prun_fast[i] >> mo) & 3) - (int)(pr_f & 3);
            switch (d)
            {
            case -1:
            case 3: { pr_f--; break; }
            case 1:
            case -3: { pr_f++; break; }
            };
            //if (pr_f != getPruning(Slice_Flip_Prun, N_SLICE1 * flip + slice)) { ERROR !!! } 
        }
        else
        {
            pr_f = getPruning(Slice_Flip_Prun, N_SLICE1 * flip + slice);
        }

        if (FAST_TWIST) // optimized away at compile time
        { // TWIST
            const int k = N_SLICE1 * twist + slice;
            const int i = k / 4;
            const int mo = 2 * (k & 3);
            int d = (int)((Slice_Twist_Prun_fast[i] >> mo) & 3) - (int)(pr_t & 3);
            switch (d)
            {
            case -1:
            case 3: { pr_t--; break; }
            case 1:
            case -3: { pr_t++; break; }
            };
            //if (pr_t != getPruning(Slice_Twist_Prun, N_SLICE1 * twist + slice)) { ERROR !!! }
        }
        else
        {
            pr_t = getPruning(Slice_Twist_Prun, N_SLICE1 * twist + slice);
        }

        newpr_f = pr_f;
        newpr_t = pr_t;
        return((pr_f > pr_t) ? pr_f : pr_t);
    }


}
/* end of file */
