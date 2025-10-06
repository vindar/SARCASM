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


namespace CubeState
	{



	/**
	 * Class that represent an elementary cube move
	 **/
	struct CubeMove
		{
		friend class CubeState;

		public:

			/** Default ctor: invalid move */
			CubeMove() { set(nullptr); }

			/** Constructor: move from string */
			CubeMove(const char* str) { set(str); }

			/** copy ctor */
			CubeMove(const CubeMove&) = default;

			/** copy operator */
			CubeMove& operator=(const CubeMove&) = default;

			/** copy from string */
			CubeMove operator=(const char* str) { set(str); return (*this); }

			/**   
            * Set the move from the begining of a string.
            * Return the position in the string that follows the move (ie next move). 
			**/
			const char* set(const char* str);

			/** Check if a move is valid */
			bool valid() const { return (_m[0] != '?'); }

			/** set the move as invalid */
            void setInvalid() { _m[0] = '?'; _m[1] = 0; _m[2] = 0; } 

            /** get the move as a string */
			const char* str() const { return _m; }

			/** the face on which the move is acting ('U', 'R', 'F', 'D', 'L', 'B') */
			char face();

			/** the type of move: (0 = clockwise, 1 = anticlockwise, 2 = halfturn) */
			int type();


		private: 

			char _m[3];
		};






	/**
     * Class that keeps track of the state of the cube.
	 **/
	class CubeState
		{
		public:

			/* ctor */
			CubeState()
				{
				_state[9 * 6] = 0; // c-string ending. 
				setSolved();
				}


			/* get the cube state*/
			const char * getState() const  { return _state; }


			/** set the state of the cube */
			void setState(const char* state);


			/** set the cube as solved */
			void setSolved();


			/** Query if the cube is solved */
			bool isSolved() const;


			/** make a move. */
			void make_move(CubeMove mv);


		private:


			char _state[9 * 6 + 1];  // the cube current state; 

			/* Move on the Up face */
			void _move_U(int type);
			void _move_U();

			/* Move on the Down face */
			void _move_D(int type);
			void _move_D();

			/* Move on the Front face */
			void _move_F(int type);
			void _move_F();

			/* Move on the Back face */
			void _move_B(int type);
			void _move_B();

			/* Move on the Left face */
			void _move_L(int type);
			void _move_L();

			/* Move on the Right face */
			void _move_R(int type);
			void _move_R();

			/* permute A -> B -> C -> D -> A */
			void _perm(int a1, int a2, int a3, int b1, int b2, int b3, int c1, int c2, int c3, int d1, int d2, int d3);
		
		};



	extern CubeState cube; // the unique CubeState object

	}
/* end of file */

