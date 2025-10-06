
#include "cache_arrays.h"

#include "arduino_undefine.h"


namespace kociemba
{

	/*****************************************
	* Array stored in flash memory (PROGMEM)
	******************************************/

#include "cache/flipMove.h"
#include "cache/twistMove.h"
#include "cache/FRtoBR_Move.h"
#include "cache/FRtoBR_Move24.h"
#include "cache/URFtoDLF_Move.h"
#include "cache/URtoDF_Move.h"
#include "cache/UBtoDF_Move.h"
#include "cache/MergeURtoULandUBtoDF.h"
#include "cache/Slice_URFtoDLF_Parity_Prun.h"
#include "cache/Slice_URtoDF_Parity_Prun.h"
#include "cache/Slice_Twist_Prun.h"
#include "cache/Slice_Flip_Prun.h"


	// *****************************************************
	// Arrays statically allocated in RAM (less than 1K total). 
	// *****************************************************
	short parityMove[2][N_MOVE] = {	// size = 72 bytes
		{ 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
		{ 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 }
	};


	short FRtoBR_Move24_2[24][N_MOVE] = { // size = 864 bytes  
		{ 0, 0, 0, 8824, 10, 8814, 5521, 1, 5520, 0, 0, 0, 3339, 5, 3340, 1904, 20, 1908 },
		{ 1, 1, 1, 8825, 11, 8815, 5520, 0, 5521, 1, 1, 1, 3338, 4, 3341, 1905, 21, 1909 },
		{ 2, 2, 2, 8820, 6, 8816, 5525, 5, 5522, 2, 2, 2, 3337, 3, 3336, 1906, 22, 1910 },
		{ 3, 3, 3, 8821, 7, 8817, 5524, 4, 5523, 3, 3, 3, 3336, 2, 3337, 1907, 23, 1911 },
		{ 4, 4, 4, 8822, 8, 8818, 5523, 3, 5524, 4, 4, 4, 3341, 1, 3338, 1902, 18, 1912 },
		{ 5, 5, 5, 8823, 9, 8819, 5522, 2, 5525, 5, 5, 5, 3340, 0, 3339, 1903, 19, 1913 },
		{ 6, 6, 6, 8816, 2, 8820, 5536, 16, 5526, 6, 6, 6, 3357, 7, 3356, 1919, 11, 1914 },
		{ 7, 7, 7, 8817, 3, 8821, 5537, 17, 5527, 7, 7, 7, 3356, 6, 3357, 1918, 10, 1915 },
		{ 8, 8, 8, 8818, 4, 8822, 5532, 12, 5528, 8, 8, 8, 3355, 11, 3358, 1917, 9, 1916 },
		{ 9, 9, 9, 8819, 5, 8823, 5533, 13, 5529, 9, 9, 9, 3354, 10, 3359, 1916, 8, 1917 },
		{ 10, 10, 10, 8814, 0, 8824, 5534, 14, 5530, 10, 10, 10, 3359, 9, 3354, 1915, 7, 1918 },
		{ 11, 11, 11, 8815, 1, 8825, 5535, 15, 5531, 11, 11, 11, 3358, 8, 3355, 1914, 6, 1919 },
		{ 12, 12, 12, 8831, 17, 8826, 5528, 8, 5532, 12, 12, 12, 3351, 22, 3347, 1897, 13, 1896 },
		{ 13, 13, 13, 8830, 16, 8827, 5529, 9, 5533, 13, 13, 13, 3350, 23, 3346, 1896, 12, 1897 },
		{ 14, 14, 14, 8829, 15, 8828, 5530, 10, 5534, 14, 14, 14, 3349, 18, 3345, 1901, 17, 1898 },
		{ 15, 15, 15, 8828, 14, 8829, 5531, 11, 5535, 15, 15, 15, 3348, 19, 3344, 1900, 16, 1899 },
		{ 16, 16, 16, 8827, 13, 8830, 5526, 6, 5536, 16, 16, 16, 3353, 20, 3343, 1899, 15, 1900 },
		{ 17, 17, 17, 8826, 12, 8831, 5527, 7, 5537, 17, 17, 17, 3352, 21, 3342, 1898, 14, 1901 },
		{ 18, 18, 18, 8809, 19, 8808, 5543, 23, 5538, 18, 18, 18, 3345, 14, 3349, 1912, 4, 1902 },
		{ 19, 19, 19, 8808, 18, 8809, 5542, 22, 5539, 19, 19, 19, 3344, 15, 3348, 1913, 5, 1903 },
		{ 20, 20, 20, 8813, 23, 8810, 5541, 21, 5540, 20, 20, 20, 3343, 16, 3353, 1908, 0, 1904 },
		{ 21, 21, 21, 8812, 22, 8811, 5540, 20, 5541, 21, 21, 21, 3342, 17, 3352, 1909, 1, 1905 },
		{ 22, 22, 22, 8811, 21, 8812, 5539, 19, 5542, 22, 22, 22, 3347, 12, 3351, 1910, 2, 1906 },
		{ 23, 23, 23, 8810, 20, 8813, 5538, 18, 5543, 23, 23, 23, 3346, 13, 3350, 1911, 3, 1907 }
	};


	// *****************************************************
	// arrays located in the (optional) 479KB buffer. 
	// -> initialized when kociemba_memory() is called
	// *****************************************************
	short_bidim_N_MOVE twistMove = (short_bidim_N_MOVE)flash_twistMove;			// of size [N_TWIST][N_MOVE] 77k
	short_bidim_N_MOVE flipMove = (short_bidim_N_MOVE)flash_flipMove;			// of size [N_FLIP][N_MOVE] 72k
	short_bidim_N_MOVE UBtoDF_Move = (short_bidim_N_MOVE)flash_UBtoDF_Move;		// of size [N_UBtoDF][N_MOVE] 47k
	short_bidim_N_MOVE FRtoBR_Move24 = (short_bidim_N_MOVE)flash_FRtoBR_Move24;	// of size [N_FRtoBR/24][N_MOVE] 18k 
	unsigned char* Slice_Twist_Prun_fast = nullptr;								// of size [N_SLICE1 * N_TWIST / 4 + 1] 265k


	// *****************************************************
	// arrays located in the (optional) 248KB buffer
	// -> initialized when kociemba_memory() is called
	// *****************************************************
	unsigned char* Slice_Flip_Prun_fast = nullptr;								// of size [N_SLICE1 * N_FLIP / 4] 248k



	FLASHMEM void set_memory(void* mem479 = nullptr, void* mem248 = nullptr)
	{
		if (mem479 != nullptr)
		{
			// mem479 contains 5  arrays. 
			twistMove = (short_bidim_N_MOVE)mem479;																// 77k 
			flipMove = (short_bidim_N_MOVE)(((short*)twistMove) + (N_TWIST * N_MOVE));							// 72k
			UBtoDF_Move = (short_bidim_N_MOVE)(((short*)flipMove) + (N_FLIP * N_MOVE));							// 47k
			FRtoBR_Move24 = (short_bidim_N_MOVE)(((short*)UBtoDF_Move) + (N_UBtoDF * N_MOVE));					// 18k
			Slice_Twist_Prun_fast = (unsigned char*)(((short*)FRtoBR_Move24) + ((N_FRtoBR / 24) * N_MOVE));		// 265k

			memcpy(twistMove, flash_twistMove, sizeof(flash_twistMove));
			memcpy(flipMove, flash_flipMove, sizeof(flash_flipMove));
			memcpy(UBtoDF_Move, flash_UBtoDF_Move, sizeof(flash_UBtoDF_Move));
			memcpy(FRtoBR_Move24, flash_FRtoBR_Move24, sizeof(flash_FRtoBR_Move24));

			memset(Slice_Twist_Prun_fast, 0, N_SLICE1 * N_TWIST / 4 + 1);
			for (int i = 0; i < N_SLICE1 * N_TWIST; i++)
				{
				int mo = 2 * (i & 3);
				unsigned char a = getPruning(Slice_Twist_Prun, i) & 3;
				Slice_Twist_Prun_fast[i / 4] += (unsigned char)(a << mo);
				}
		}
		else
		{ // use arrays in progmem
			twistMove = (short_bidim_N_MOVE)flash_twistMove;
			flipMove = (short_bidim_N_MOVE)flash_flipMove;
			UBtoDF_Move = (short_bidim_N_MOVE)flash_UBtoDF_Move;
			FRtoBR_Move24 = (short_bidim_N_MOVE)flash_FRtoBR_Move24;
			Slice_Flip_Prun_fast = nullptr;
		}

		if (mem248 != nullptr)
		{
			// mem248 contain a single array
			Slice_Flip_Prun_fast = (unsigned char*)mem248;	// 248k
			memset(Slice_Flip_Prun_fast, 0, N_SLICE1 * N_FLIP / 4);
			for (int i = 0; i < N_SLICE1 * N_FLIP; i++)
				{
				int mo = 2 * (i & 3);
				unsigned char a = getPruning(Slice_Flip_Prun, i) & 3;
				Slice_Flip_Prun_fast[i / 4] += (unsigned char)(a << mo);
				}
		}
		else
		{ // use array in progmem
			Slice_Flip_Prun_fast = nullptr;
		}
	}

}
/* end of file */



