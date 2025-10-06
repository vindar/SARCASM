#pragma once 

#include "arduino_undefine.h"

namespace kociemba
{

	/**
	* reserve additional RAM memory for speeding up computations (optional).
	*
	* - mem479 : pointer to a (16 bytes aligned ?) memory buffer of size at least 479KB
	* - mem248 : pointer to a (16 bytes aligned ?) memory buffer of size at least 248KB
	*
	* Once set, the memory buffers are used for all subsequent calls to solve() until
	* they are removed by calling this method again and passing nullptr in place of the
	* buffer's adress.
	*
	* On teensy 4.1 on should: 
	* - allocate mem479 via new/malloc (RAM2 : DMAMEM)
	* - allocate  mem248 as a static array in global memory (RAM1)
	*
	* Using both buffer speeds up the computation by a factor of 4 on Teensy 4.1.
	**/
	void set_memory(void* mem479 = nullptr, void* mem248 = nullptr);



	/**
	 * Solve a cube.
	 *
	 * @param facelets
	 *          is the cube definition string, see facelet.h for details.
	 *
	 * @param maxDepth
	 *          defines the maximal allowed maneuver length. [24 is a good value for teensy].
	 *
	 *@param timeOut
	 *          defines the maximum computing time of the method in milliseconds.
	 *
	 * @param useSeparator
	 *          determines if a " . " separates the phase1 and phase2 parts of the solver string like in F' R B R L2 F .
	 *          U2 U D for example.<br>
	 *
	 * Return a string with the solution or nullptr if no solution was found (timeout) or cube not solvable.
	 *
	 * the return string is static and must not be freed. 
	 */
	const char* solve(const char* facelets, int maxDepth = 24, int timeOut = 10000, int useSeparator = 0);

}

/* end of file */

