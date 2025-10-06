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
// along with this code. If not, see <http://www.gnu.org/licenses/>.#pragma once

#pragma once


namespace CubeOrientation
    {



    /**
    * Class that keeps track of the cube orientation during moves. *
    *
    * - face Up    (U) is the one that is on top of the cube
    * - face Right (R) is the one on the right (when the user is looking at the font face)
    * - face Front (F) is the one that is in front of the user (not the camera).
    * - face Down  (D) is the one sitting on the craddle
    * - face Left  (L) is the one on the left (when the user is looking at the font face)
    * - face Back  (B) is the one that is behind (the one the camera see)
    **/
    class CubeOrientation
        {

        public:

            /** ctor */
            CubeOrientation();


            /** reset the cube to a given position */
            void reset(char up = 'U', char right = 'R', char front = 'F', char down = 'D', char left = 'L', char back = 'B');


            /**
             * return the current location of a face
             *
             * @param  face    The face ('U', 'R', 'F', 'D', 'L', 'B')
             *
             * @returns    the location of that face (0=up, 1=right, 2=front, 3=down, 4=left, 5=back)
             */
            int whereis(char face);


            /** return which face is currently showing on up (top side) */
            inline char whois_up() { return _cpos[0]; }

            /** return which face is currently showing on the right (from the POV of the user) */
            inline char whois_right() { return _cpos[1]; }

            /** return which face is currently showing on the front (user side) */
            inline char whois_front() { return _cpos[2]; }

            /** return which face is currently showing down (craddle side) */
            inline char whois_down() { return _cpos[3]; }

            /** return which face is currently showing on the left  (from the POV of the user)*/
            inline char whois_left() { return _cpos[4]; }

            /** return which face is currently showing on the back (camera side) */
            inline char whois_back() { return _cpos[5]; }


            /**
             * Return the current front face rotation when looking directly at it.
             * - return a rotation between 0 and 3.
             * - return 0 if face index (fi) 1 in on the top left.
             * - otherwise clockwise rotation: return 1 if fi=top right, 2 if fi=bottom right, 3 if fi=bottom left
             **/
            int front_rotation();

            /**
             * Return the current back face rotation when looking directly at it (like the camera)
             * - return a rotation between 0 and 3.
             * - return 0 if face index (fi) 1 in on the top left.
             * - otherwise clockwise rotation: return 1 if fi=top right, 2 if fi=bottom right, 3 if fi=bottom left
             **/
            int back_rotation();

            /**
             * Return the current top face rotation when looking directly at it, over and a little in front of the machine.
             * - return a rotation between 0 and 3.
             * - return 0 if face index (fi) 1 in on the top left.
             * - otherwise clockwise rotation: return 1 if fi=top right, 2 if fi=bottom right, 3 if fi=bottom left
             **/
            int up_rotation();

            /**
             * Return the current bottom face rotation when looking directly at it, under and a little in front of the machine.
             * - return a rotation between 0 and 3.
             * - return 0 if face index (fi) 1 in on the top left.
             * - otherwise clockwise rotation: return 1 if fi=top right, 2 if fi=bottom right, 3 if fi=bottom left
             **/
            int down_rotation();

            /**
             * Return the current left face rotation when looking directly at it.
             * - return a rotation between 0 and 3.
             * - return 0 if face index (fi) 1 in on the top left.
             * - otherwise clockwise rotation: return 1 if fi=top right, 2 if fi=bottom right, 3 if fi=bottom left
             **/
            int left_rotation();

            /**
             * Return the current right face rotation when looking directly at it.
             * - return a rotation between 0 and 3.
             * - return 0 if face index (fi) 1 in on the top left.
             * - otherwise clockwise rotation: return 1 if fi=top right, 2 if fi=bottom right, 3 if fi=bottom left
             **/
            int right_rotation();



            /** make a quarter clockwise turn */
            void turn_clockwise();

            /** make a quarter anticlockwise turn */
            void turn_anticlockwise();

            /** make half a turn */
            void turn_half();

            /***tumble the cube */
            void tumble();


            /** make a moves.
             * - 'T' : tumble
             * - 'R' : clockwise rotation
             * - 'V' : anticlockwise rotation
             * - 'A' : half turn
             **/
            void make_move(char c);

            /** make a sequnece of moves. if len not specified, use c-string lenght */
            void make_moves(const char* moves, int len = -1);



            /** find moves to put a given face on top */
            const char* find_move_for_top(char face);




        private:


            char _cpos[6]; // pos[0] = "R" means that the face currently on top is "R"   


            /** compute a face rotation */
            int get_rot(char facelook, char faceup);
        };



    // the cube orientation object. 
    extern CubeOrientation cube; 

    }

/* end of file */ 



