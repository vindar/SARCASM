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


#include "CubeState.h"
#include "config.h"
#include "debug.h"
#include "misc.h"
#include "panic.h"

#include <Arduino.h>



namespace CubeState
	{


	CubeState cube; 


	typedef enum
		{
		iU1, iU2, iU3, iU4, iU5, iU6, iU7, iU8, iU9,
		iR1, iR2, iR3, iR4, iR5, iR6, iR7, iR8, iR9,
		iF1, iF2, iF3, iF4, iF5, iF6, iF7, iF8, iF9,
		iD1, iD2, iD3, iD4, iD5, iD6, iD7, iD8, iD9,
		iL1, iL2, iL3, iL4, iL5, iL6, iL7, iL8, iL9,
		iB1, iB2, iB3, iB4, iB5, iB6, iB7, iB8, iB9
		} facelet_t;




	SLOWFUN const char* CubeMove::set(const char* str)
		{
		if (str == nullptr) { setInvalid();  return nullptr; }
		_m[0] = misc::uppercase(str[0]);
		if ((_m[0] != 'U') && (_m[0] != 'R') && (_m[0] != 'F') && (_m[0] != 'D') && (_m[0] != 'L') && (_m[0] != 'B')) { setInvalid(); return nullptr; }
		str++;
		_m[1] = str[0];
		_m[2] = 0;
		switch (_m[1])
			{
			case 0:    { return str; }
			case ' ':  { str++; return str; }
			case '\'': { str++; return str; }
			case '2':  { str++; return str; }
			}
		setInvalid(); 
		return nullptr;
		}



	/** the face on which the move is acting ('U', 'R', 'F', 'D', 'L', 'B') */
	char CubeMove::face() { return _m[0]; }

	/** the type of move: (0 = clockwise, 1 = anticlockwise, 2 = halfturn) */
	int CubeMove::type()
		{
		switch (_m[1])
			{
			case 0: return 0;
			case ' ': return 0;
			case '\'': return 1;
			case '2': return 2;
			default: ABORT("CubeMove::type() error");
			}
		return 0;
		}



	/*
	std::string moveToString(const std::vector<cubeMove>& moves)
		{
		std::string res = "";
		for (const cubeMove& cm : moves)
			{
			if (res.length() > 0) { res += " "; }
			res += cm.str();
			}
		return res;
		}



	std::pair<std::vector<cubeMove>, bool> parseMoves(const std::string& str)
		{
		std::vector<cubeMove> res;
		const char* p = str.c_str();
		while (p[0] != 0)
			{
			if (p[0] == ' ')
				{
				p++;
				} else
				{
				cubeMove cm;
				p = cm.set(p);
				if (p == nullptr)
					{
					res.clear();
					return { res, false };
					}
				res.push_back(cm);
				}
			}
		return { res, true };
		}


	std::pair<std::vector<cubeMove>, bool> CubeState::findSolution()
		{
		elapsedMillis em = 0;
		cout << "Looking for solution....";
		char buf[55];
		memcpy(buf, _state, 55);
		char* sol = solution(buf, 24, 1000, 0, "cache");
		if (sol == NULL)
			{
			cout << " unsolvable !!!\n";
			std::vector<cubeMove> res;
			return { res, false };
			}
		auto r = parseMoves(sol);
		free(sol);
		cout << " found in " << (int)em << " ms\n";
		return r;
		}

		*/



	SLOWFUN bool CubeState::isSolved() const 
		{
		for (int i = 0; i < 6; i++)
			{
			int off = 9 * i;
			char c = _state[off];
			for (int j = 1; j < 9; j++)
				{
				if (_state[off + j] != c) return false;
				}
			}
		return true;
		}



	void CubeState::setState(const char* state)
		{
		memcpy(_state, state, 9 * 6);
		}


	/** set the cube as solved */
	void CubeState::setSolved()
		{
		memcpy(_state, "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB", 9 * 6);
		}


	/** make a move. */
	SLOWFUN void CubeState::make_move(CubeMove mv)
		{
		if (mv.valid() == false) { ABORT("CubeState::make_move: invalid move"); }
		int type = 0;
		switch (mv._m[1])
			{
			case 0: { break; }
			case ' ': { break; }
			case '\'': { type = 1; break; }
			case '2': { type = 2; break; }
			default: { ABORT("CubeState::make_move: parse error 1"); }
			}
		switch (mv._m[0])
			{
			case 'U': { _move_U(type); break; }
			case 'R': { _move_R(type); break; }
			case 'F': { _move_F(type); break; }
			case 'D': { _move_D(type); break; }
			case 'L': { _move_L(type); break; }
			case 'B': { _move_B(type); break; }
			default: { ABORT("CubeState::make_move: parse error 2"); }
			}
		}



	SLOWFUN void CubeState::_perm(int a1, int a2, int a3, int b1, int b2, int b3, int c1, int c2, int c3, int d1, int d2, int d3)
		{
		char _prevstate[9 * 6];
		memcpy(_prevstate, _state, 9 * 6);

		_state[b1] = _prevstate[a1];
		_state[b2] = _prevstate[a2];
		_state[b3] = _prevstate[a3];

		_state[c1] = _prevstate[b1];
		_state[c2] = _prevstate[b2];
		_state[c3] = _prevstate[b3];

		_state[d1] = _prevstate[c1];
		_state[d2] = _prevstate[c2];
		_state[d3] = _prevstate[c3];

		_state[a1] = _prevstate[d1];
		_state[a2] = _prevstate[d2];
		_state[a3] = _prevstate[d3];
		}



	/* Move on the Up face */
	void CubeState::_move_U(int type)
		{
		switch (type)
			{
			case 0: { _move_U(); return; }
			case 1: { _move_U(); _move_U(); _move_U(); return; }
			case 2: { _move_U(); _move_U(); return; }
			}
		ABORT("CubeState::_move_U type error");
		}


	void CubeState::_move_U()
		{
		_perm(iU1, iU2, iU3, iU3, iU6, iU9, iU9, iU8, iU7, iU7, iU4, iU1);
		_perm(iB3, iB2, iB1, iR3, iR2, iR1, iF3, iF2, iF1, iL3, iL2, iL1);
		}



	void CubeState::_move_D(int type)
		{
		switch (type)
			{
			case 0: { _move_D(); return; }
			case 1: { _move_D(); _move_D(); _move_D(); return; }
			case 2: { _move_D(); _move_D(); return; }
			}
		ABORT("CubeState::_move_D type error");
		}

	void CubeState::_move_D()
		{
		_perm(iD1, iD2, iD3, iD3, iD6, iD9, iD9, iD8, iD7, iD7, iD4, iD1);
		_perm(iF7, iF8, iF9, iR7, iR8, iR9, iB7, iB8, iB9, iL7, iL8, iL9);
		}



	void CubeState::_move_F(int type)
		{
		switch (type)
			{
			case 0: { _move_F(); return; }
			case 1: { _move_F(); _move_F(); _move_F(); return; }
			case 2: { _move_F(); _move_F(); return; }
			}
		ABORT("CubeState::_move_F type error");
		}

	void CubeState::_move_F()
		{
		_perm(iF1, iF2, iF3, iF3, iF6, iF9, iF9, iF8, iF7, iF7, iF4, iF1);
		_perm(iL9, iL6, iL3, iU7, iU8, iU9, iR1, iR4, iR7, iD3, iD2, iD1);
		}



	void CubeState::_move_B(int type)
		{
		switch (type)
			{
			case 0: { _move_B(); return; }
			case 1: { _move_B(); _move_B(); _move_B(); return; }
			case 2: { _move_B(); _move_B(); return; }
			}
		ABORT("CubeState::_move_B type error");
		}

	void CubeState::_move_B()
		{
		_perm(iB1, iB2, iB3, iB3, iB6, iB9, iB9, iB8, iB7, iB7, iB4, iB1);
		_perm(iU3, iU2, iU1, iL1, iL4, iL7, iD7, iD8, iD9, iR9, iR6, iR3);
		}



	void CubeState::_move_L(int type)
		{
		switch (type)
			{
			case 0: { _move_L(); return; }
			case 1: { _move_L(); _move_L(); _move_L(); return; }
			case 2: { _move_L(); _move_L(); return; }
			}
		ABORT("CubeState::_move_L type error");
		}

	void CubeState::_move_L()
		{
		_perm(iL1, iL2, iL3, iL3, iL6, iL9, iL9, iL8, iL7, iL7, iL4, iL1);
		_perm(iU1, iU4, iU7, iF1, iF4, iF7, iD1, iD4, iD7, iB9, iB6, iB3);
		}



	void CubeState::_move_R(int type)
		{
		switch (type)
			{
			case 0: { _move_R(); return; }
			case 1: { _move_R(); _move_R(); _move_R(); return; }
			case 2: { _move_R(); _move_R(); return; }
			}
		ABORT("CubeState::_move_R type error");
		}

	void CubeState::_move_R()
		{
		_perm(iR1, iR2, iR3, iR3, iR6, iR9, iR9, iR8, iR7, iR7, iR4, iR1);
		_perm(iU9, iU6, iU3, iB1, iB4, iB7, iD9, iD6, iD3, iF9, iF6, iF3);
		}


	}



/* endif */

