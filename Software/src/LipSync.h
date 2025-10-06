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
//
#pragma once

#include <Arduino.h>
#include <teensythreads.h>

namespace LipSync
	{
	/** called by update to update the audio info needed for lip syncing */
	void lipsync_isr_process_block(const int16_t* samples, int N);
	
	bool speaking_recent();
		
	enum MouthState : uint8_t { M_CLOSE = 0, M_NEUTRAL = 1, M_OPEN = 2 };

	/**
    * Return the current mouth state
	**/
	MouthState mouthState();

	}


/** end of file */