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

#include "CubeRead.h"
#include "CubeReadConfig.h"
#include "Hardware.h"
#include "config.h"
#include "debug.h"
#include "panic.h"
#include "Memory.h"

#include "LedStrip.h"
#include "BasicMoves.h"
#include "DRV8834.h"
#include "ESPCAM.h"
#include "display.h"
#include "audioMAX98357A.h"
#include "AudioLed.h"

#include "CubeOrientation.h"
#include "CubeState.h"
#include "graphics3D.h"
#include "Console.h"
#include "sentences.h"

#include <TeensyThreads.h>
#include <tgx.h>


namespace CubeRead
    {


    // Beep when starting a solve. 
    const char FILENAME_SOUND_START_SOLVE[] PROGMEM = "act_sounds/start_solve1.wav";





    // list of all facets, order: U R F D L B
    typedef enum
        {
        fU1, fU2, fU3, fU4, fU5, fU6, fU7, fU8, fU9, 
        fR1, fR2, fR3, fR4, fR5, fR6, fR7, fR8, fR9,
        fF1, fF2, fF3, fF4, fF5, fF6, fF7, fF8, fF9, 
        fD1, fD2, fD3, fD4, fD5, fD6, fD7, fD8, fD9, 
        fL1, fL2, fL3, fL4, fL5, fL6, fL7, fL8, fL9, 
        fB1, fB2, fB3, fB4, fB5, fB6, fB7, fB8, fB9
        } facelet_t;

    // index of the 6 centers, order: U R F D L B
    const int _iCENTERS[6]  = { fU5, fR5, fF5, fD5, fL5, fB5 };

    // index of the 24 edges, order: U R F D L B
    const int _iEDGES[24]   = { fU2, fU4, fU6, fU8, fR2, fR4, fR6, fR8, fF2, fF4, fF6, fF8, fD2, fD4, fD6, fD8, fL2, fL4, fL6, fL8, fB2, fB4, fB6, fB8 };

    // index of the 24 corners, order: U R F D L B 
    const int _iCORNERS[24] = { fU1, fU3, fU7, fU9, fR1, fR3, fR7, fR9, fF1, fF3, fF7, fF9, fD1, fD3, fD7, fD9, fL1, fL3, fL7, fL9, fB1, fB3, fB7, fB9 };

    // color of the stickers, ordered U0,...,U8,R0,...,R8,F0,...,F8,D0,...,D8,L0,...,L8,B0,...,B8
    // this is the color after luminosity correction.
    tgx::fVec3 _sticker_color[9*6];

    // the color variance for each of the center sticker. 
    float _sticker_center_variance[6];

    // the color of the sticker given by its index 0=U, 1=R, 2=F, 3=D, 4=L, 5=B or a negative number if not assigned yet. 
    int8_t _sticker_ind[9*6];

    tgx::fVec3 _face_colors[6]; 


    /** apply luminosity correction for the color of a given facet */
    tgx::fVec3 _luminosityCorrection(tgx::fVec3 c, int facet)
        {
        const float z = c.x + c.y + c.z;
        tgx::fVec3 scale = luminosity_corr[facet].a + (luminosity_corr[facet].b * z);   // (a + b·Z)
        tgx::fVec3 shift = luminosity_corr[facet].c + (luminosity_corr[facet].d * z);   // (c + d·Z)
        return (scale * c) + shift; // produit composante par composante + translation
        }


    /** Compute the mean color (normalized in [0,1] inside a given rectangle */
    tgx::fVec3 _mean(const tgx::Image<tgx::RGB32>& im, const tgx::iBox2& R)
        {
        tgx::iVec3 sum(0, 0, 0);
        for (int i = R.minX; i <= R.maxX; i++)
            {
            for (int j = R.minY; j <= R.maxY; j++)
                {
                const tgx::RGB32 c = im(i, j);
                sum += tgx::iVec3(c.R, c.G, c.B);
                }
            }
        const int nbp = ((R.maxX - R.minX + 1) * (R.maxY - R.minY + 1));
        return tgx::fVec3((float)sum.x / (255.0f * nbp), (float)sum.y / (255.0f * nbp), (float)sum.z / (255.0f * nbp));
        }


    /** Compute the (normalized) variance inside a given rectangle */
    tgx::fVec3 _variance(const tgx::Image<tgx::RGB32>& im, const tgx::iBox2& R)
        {
        tgx::fVec3 sum(0, 0, 0);
        const tgx::fVec3 m = _mean(im, R);
        for (int i = R.minX; i <= R.maxX; i++)
            {
            for (int j = R.minY; j <= R.maxY; j++)
                {
                const tgx::RGB32 c = im(i, j);
                const tgx::fVec3 v((float)c.R / 255.0f, (float)c.G / 255.0f, (float)c.B / 255.0f);
                sum += (v - m) * (v - m);
                }
            }
        const float nbp = (float)((R.maxX - R.minX + 1) * (R.maxY - R.minY + 1));
        return sum / nbp;
        }


    const char* cubeString()
        {
        static char str[55];
        str[54] = 0;
        for (int i = 0; i < 54; i++)
            {
            switch (_sticker_ind[i])
                {
                case 0: str[i] = 'U'; break;
                case 1: str[i] = 'R'; break;
                case 2: str[i] = 'F'; break;
                case 3: str[i] = 'D'; break;
                case 4: str[i] = 'L'; break;
                case 5: str[i] = 'B'; break;
                default: str[i] = ' '; break; // TODO : add debug here.                     
                }
            }
        return str;
        }


    void readFace(const tgx::Image<tgx::RGB32>& im, int face, int rot)
        {
        static const int antir[4] = { 90, 0 , 270, 180 }; // recall that the image taken by the cam in already 90 counterclockwise (landscape)
        const int anglerot = antir[rot]; 
        static const int rotation_array[4][9] = { {0,1,2,3,4,5,6,7,8}, {6,3,0,7,4,1,8,5,2}, {8,7,6,5,4,3,2,1,0}, {2,5,8,1,4,7,0,3,6} };
        for (int i = 0; i < 9; i++)
            {
            int ind = (9 * face) + rotation_array[rot][i];
            _sticker_color[ind] = _luminosityCorrection(_mean(im, cR[i]), i);
            if (anglerot != 0)
                {                 
                graphics3D::stickerImage(54).copyFrom(im.getCrop(cRbig[i]));
                graphics3D::stickerImage(ind).blitRotated(graphics3D::stickerImage(54), { 0,0 }, anglerot, 1.0f);
                }
            else
                {
                graphics3D::stickerImage(ind).copyFrom(im.getCrop(cRbig[i]));
                }
            }
        _sticker_center_variance[face] = _variance(im, cR[4]).norm();
        return;
        }


    /** distance between two color */
    float _dist(tgx::fVec3 a, tgx::fVec3 b)
        {
        return tgx::dist2(a, b);
        }


    /**
     * find the index of the centroid closest to a given color.
     *
     * @param   a           the color of the sticker.
     * @param   centroid    the array of centroids to use.
     * @param   count       the number of stickers currently assigned to each centroid.
     * @param   count_max   the maximum number of stickers that can be assigned to a centroid.
     *
     * @returns the centroid to which the sticker should be assigned (or -1 if none countable)
     */
    int8_t _closest(tgx::fVec3 a, const std::array<tgx::fVec3, 6>& centroid, const std::array<int, 6>& count, const std::array<int, 6>& count_max)
        {
        int8_t mini = -1;
        float dmin = std::numeric_limits<float>::max();
        for (int8_t u = 0; u < 6; u++)
            {
            if (count[u] < count_max[u])
                {
                const float d = _dist(a, centroid[u]);
                if ((d < dmin) || (mini == -1)) { dmin = d; mini = u; }
                }
            }
        return mini;
        }


    /**
     * Among all the sticker associated with a given label/centroid. Find the one that is furthest from the centroid.
     *
     * @param   indtab      the array of indices of stickers to consider
     * @param   nb          the size of the array of sticker indices
     * @param   centroid    the array of centroids to use.
     * @param   label       The label to consider
     *
     * @returns the index of the sticker with label 'label' that is furthest from the 'centroid'. Return -1 if none found.
     */
    int _furthest(const int* indtab, int nb, tgx::fVec3 centroid, int8_t label)
        {
        float dmax = 1;
        int maxi = -1;
        for (int n = 0; n < nb; n++)
            {
            const int ind = indtab[n];
            if (_sticker_ind[ind] == label) // only consider the stickers with the same label
                {
                const float d = _dist(_sticker_color[ind], centroid); // distance to the centroid
                if ((d > dmax) || (maxi == -1)) // find the sticker with the max distance to the centroid
                    {
                    dmax = d;
                    maxi = ind;
                    }
                }
            }
        return maxi; // return the index of the sticker with the max distance to the centroid
        };


    /**
     * Label all the currently unassigned sticker indexed by 'indtab' to the closest centroid that
     * has less than count_max stickers assigned to it.
     *
     * @param   indtab      array of the index of the stickers to label.
     * @param   nb          size of the array of sticker indices.
     * @param   centroid    the array of centroids to use.
     * @param   count       the number of stickers currently assigned to each centroid.
     * @param   count_max   the maximum number of stickers that can be assigned to a centroid.
     */
    void _cluster(const int* indtab, int nb, const std::array<tgx::fVec3, 6>& centroid, std::array<int, 6>& count, const std::array<int, 6>& count_max)
        {
        for (int m = 0; m < nb; m++)
            {
            const int ind = indtab[m];
            if (_sticker_ind[ind] >= 0) continue; // sticker already assigned
            const int8_t cl = _closest(_sticker_color[ind], centroid, count, count_max);
            if (cl < 0)
                { // should never happen
                ABORT("Impossible error in CubeRead::_cluster() !");
                }
            count[cl]++; // increment the number of stickers assigned to the centroid
            _sticker_ind[ind] = cl; // assign the sticker to the centroid
            }
        };


    /**
     * Unassign the stickers stickers until all centroid have, at most, max_count assigned stickers.
     *
     * @param   indtab      array of the index of the stickers to consider.
     * @param   nb          size of the array of sticker indices.
     * @param   centroid    the array of centroids to use.
     * @param   count       the number of stickers currently assigned to each centroid.
     * @param   count_max   the maximum number of stickers that can be assigned to a centroid.
     */
    void _shave(const int* indtab, int nb, const std::array<tgx::fVec3, 6>& centroid, std::array<int, 6>& count, const std::array<int, 6>& count_max)
        {
        for (int8_t u = 0; u < 6; u++)
            {
            while (count[u] > count_max[u])
                { // unassign stickers from this centroid until it has at most count_max stickers
                const int ind = _furthest(indtab, nb, centroid[u], u); // find the sticker with the max distance to the centroid
                if (ind < 0)
                    { // should never happen
                    ABORT("Impossible error in CubeRead::_shave() !");
                    }
                _sticker_ind[ind] = -1; // unassign the sticker
                count[u]--; // decrement the number of stickers assigned to the centroid
                }
            }
        }


    /** return true if all the element in count, except at index mi, are equal to val */
    bool equalcount(const std::array<int, 6>& count, int val, int mi)
        {
        for (int i = 0; i < 6; i++)
            {
            if (i == mi) continue; // skip the special face
            if (count[i] != val) return false; // not equal
            }
        return true;
        }


    /**
     * Label the stickers indexed by 'indtab'.
     *
     * @param   indtab  array if indices of the sticker to label.
     * @param   nb      size of the array
     * @param   mi      index of the special face (with most variance) to treat separately.
     *
     * @returns True if it succeeds, false if it fails.
     */
    bool _findConfiguration(const int* indtab, int nb, int mi)
        {
        // initially, all stickers are unassigned
        for (int i = 0; i < nb; i++) { _sticker_ind[indtab[i]] = -1; }


        // the inital centroids are the center face stickers
        std::array<tgx::fVec3, 6> centroid;
        for (int i = 0; i < 6; i++) { centroid[i] = _sticker_color[_iCENTERS[i]]; }

        // and they have no one assigned to them yet
        std::array<int, 6> count = { 0, 0, 0, 0, 0, 0 };


        std::array<int, 6> count_max = { nb,nb,nb,nb,nb,nb }; // no limit
        count_max[mi] = 0; // except for the special face

        // perform the initial clustering until all face, except the special face, have exactly 4 stickers assigned to them
        while (!equalcount(count, 4, mi))
            { // all faces have at most 4 stickers assigned to them but their are 
            // still some faces with less than 4 stickers assigned to them (and the special face has no sticker assigned to it)            
            _cluster(indtab, nb, centroid, count, count_max);

            // and now unassign stickers stickers so that each centroid has at most 4 stickers (and the special face 0)
            for (int i = 0; i < 6; i++) count_max[i] = ((i == mi) ? 0 : 4);
            _shave(indtab, nb, centroid, count, count_max);

            // prevent further modification of completed face (4 assigned) but set no limit on the other ones (except for the special face which is still not allowed)
            for (int i = 0; i < 6; i++) count_max[i] = ((i == mi) ? 0 : ((count[i] < 4) ? nb : 4));
            }

        // ok, finally assign the remaining 4 unassigned sitckers to the special face
        for (int i = 0; i < nb; i++)
            {
            if (_sticker_ind[indtab[i]] < 0) // sticker not assigned yet
                {
                _sticker_ind[indtab[i]] = mi; // assign it to the special face
                count[mi]++; // increment the number of stickers assigned to the centroid
                }
            }

        // assignement completed !!!
        // possible improvement: now that we have our initial cnetroids, we may perform a few additional iteration of the classical k-means 
        // algorithm using the center of mass of the cluster as the new centroid and then finish with the same procedure of shaving and reassigning

        return true;
        }


    bool findConfiguration()
        {
        // the center stickers are assigned to themselves
        for (int i = 0; i < 6; i++) { _sticker_ind[_iCENTERS[i]] = i; }

        // we find the excluded center (the one with max variance).         
        int mi = 0;
        float mv = _sticker_center_variance[0];
        for (int i = 1; i < 6; i++)
            {
            if (_sticker_center_variance[i] >= mv) { mv = _sticker_center_variance[i]; mi = i; }
            }

        // find the label for the edge stickers
        if (!_findConfiguration(_iEDGES, 24, mi)) return false;

        // find the color for the corner stickers
        if (!_findConfiguration(_iCORNERS, 24, mi)) return false;

        // ok ! we can compute the average color for each face               
        for (int i = 0; i < 6; i++) { _face_colors[i] = tgx::fVec3(0, 0, 0); }
        for (int i = 0; i < 9*6; i++)
            {
            if ((i % 9) != 4) // excluded center faces. 
                {
                _face_colors[_sticker_ind[i]] += (_sticker_color[i] / 8.0f); // add the color of the sticker to the face color
                }
            }    
        return true;
        }


    tgx::RGB565 faceColor(int face)
        {
        if (face < 0 || face > 5) { ABORT("CubeRead::faceColor() : invalid face index: " << face); }
        return tgx::RGB565(_face_colors[face]);
        }


    tgx::RGB565 stickerAverageColor(int sticker)
        {
        if (sticker < 0 || sticker > 53) { ABORT("CubeRead::sticker_avg_color() : invalid sticker index: " << sticker); }
        const int f = _sticker_ind[sticker];
        if (f < 0) { ABORT("CubeRead::sticker_avg_color() : sticker not assigned: " << sticker); }
        return faceColor(f);
        }



    static void lightOn()
        {
        LedStrip::enableLedAudio(false);
        for (int i = 0; i < LED_COUNT; i++)
            {
            if ((i >= cube_read_minled) && (i <= cube_read_maxled))
                {
                LedStrip::setLed(i, cube_read_R, cube_read_G, cube_read_B, cube_read_W);
                } 
            else
                {
                LedStrip::setLed(i, 0, 0, 0, 0);
                }
            }
        LedStrip::show();
        }


    static void lightOff()
        {
        LedStrip::enableLedAudio(true);
        }




    const int CUBE_3D_INTRO_MS = 800;
    const float CUBE_3D_INTRO_NB_TURN = 1;


    SLOWFUN static void _playIntroCube(int em)
        {
        float r = ((int)em) / ((float)CUBE_3D_INTRO_MS);
        if (r < 0.01f) r = 0.01f;
        if (r > 1.0f) r = 1.0f;
        const float rot = (360 * CUBE_3D_INTRO_NB_TURN * r);
        tgx::fMat4 M;
        M.setScale(r, r, r);
        M.multRotate(rot, { 0.0f, 1.0f, 0.0f });
        graphics3D::updateAll(1, 0, &M);
        threads.yield();
        }



    // the camera image in extmem
    static tgx::Image<tgx::RGB32> _imcam((tgx::RGB32*)Memory::camerabuffer_extmem, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LX, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LY);
    
    inline bool intersects(const tgx::iBox2 & R1, const tgx::iBox2& R2) 
        {        
        return !((R1.maxX < R2.minX) || (R2.maxX < R1.minX) || (R1.maxY < R2.minY) || (R2.maxY < R1.minY));
        }
        
    /** this callback only copies to extmem the pixels that belong to the cube 'big' facets' to save a little time */
    static int _capture_image_cube(JDEC* jdec, void* bitmap, JRECT* rect)
        {
        tgx::iBox2 B(rect->left, rect->right, rect->top, rect->bottom);
        for (int j = 0; j < 9; j++)
            {
            if (intersects(B, cRbig[j])) 
                {
                uint8_t* p = (uint8_t*)bitmap;
                for (int j = rect->top; j <= rect->bottom; j++)
                    {
                    for (int i = rect->left; i <= rect->right; i++)
                        {
                        _imcam(i, j) = tgx::RGB32(p[0], p[1], p[2]);
                        p += 3;
                        }
                    }
                return 1;
                }
            }
        return 1;
        }
    



    /**
    * Take pictures, rotate the cube and find the configuration.
    * 
    * return:
    * - 0 if success,   
    * - 1 if the cube is missing  
    * - 2 if the cube is colors could not be read
    **/
    int readCubeAndFindConfiguration()
        {

        // play the starting solve 'beep'
        MAX98357A::stopSpeak(); 
        MAX98357A::stopMusic();
        MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC); // set the audio LED driver to voice mode
        MAX98357A::setAudioLevelMusic(START_SOLVE_SOUND_LEVEL);
        MAX98357A::playMusic(FILENAME_SOUND_START_SOLVE);
        Console::console.clear();

        graphics3D::cube3D.resetPosition(); 
        graphics3D::resetBlankStickers(true, false);

        graphics3D::cube3D.turn(2); // we take the place of the camera. 
       
        graphics3D::setRenderer(0, Display::image_main); // start with big cube in middle of screen. 

        // turn on the motor driver
        DRV8834::enable(true);

        elapsedMillis em = 0; 

        BasicMoves::align_start(); 
        while (BasicMoves::align_progress() < 1.0f) { _playIntroCube((int)em); }

        BasicMoves::closeGrip_start(false, true);
        while (BasicMoves::closeGrip_progress() < 1.0f) { _playIntroCube((int)em); }

        if (BasicMoves::missingCube()) 
            { 
            BasicMoves::openGrip_start(true, true);
            while (BasicMoves::openGrip_progress() < 1.0f) { _playIntroCube((int)em); }
            DRV8834::enable(false);
            return 1; 
            }

        BasicMoves::openGrip_start(false, true);

        MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE); // set the audio LED driver to voice mode
        MAX98357A::speak(sentences::picker_solve_start.next_str());

 
        // 0 1 2 3 4 5
        // U R F D L B
        int face[6] = { 2,1,5,4,3,0 };
        int rot[6] = { 0,0,0,0,1,1 };

        int lon = 0;

        lightOn();
        em = 0; while (em < 50) { graphics3D::updateAll(1); } // wait 50ms for light to be fully on.


        // take the first picture
        ESPCAM::capture(_capture_image_cube, -1, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LX, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LY);
        lightOff();
        readFace(_imcam, face[0], rot[0]);

        while (BasicMoves::openGrip_progress() < 1.0f) { graphics3D::updateAll(1); } // make sure the grip is completely open

        BasicMoves::turn_start(1);         
        lon = 0; 
        while (BasicMoves::turn_progress() < 1.0f) 
            { 
            graphics3D::updateAll(3, 1);
            if ((lon == 0) && (DRV8834::time_until_turn_completion() < 50000)) { lon = 1; em = 0; lightOn();}
            } 
        graphics3D::cube3D.turn(1);
        if (lon == 0) { lightOn(); }
        while (em < 50) { graphics3D::updateAll(1); }

        // take the second picture
        ESPCAM::capture(_capture_image_cube, -1, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LX, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LY);
        lightOff();
        readFace(_imcam, face[1], rot[1]);

        BasicMoves::turn_start(1);
        lon = 0;
        while (BasicMoves::turn_progress() < 1.0f)
            {
            graphics3D::updateAll(3, 1);
            if ((lon == 0) && (DRV8834::time_until_turn_completion() < 50000)) { lon = 1; em = 0; lightOn(); }
            }
        graphics3D::cube3D.turn(1);
        if (lon == 0) { lightOn(); }
        while (em < 50) { graphics3D::updateAll(1); }


        lightOn();
        em = 0; while (em < 50) { graphics3D::updateAll(1); } // wait 50ms for light to be fully on.

        // take the third picture
        ESPCAM::capture(_capture_image_cube, -1, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LX, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LY);
        lightOff();
        readFace(_imcam, face[2], rot[2]);


        BasicMoves::turn_start(1);
        lon = 0;
        while (BasicMoves::turn_progress() < 1.0f)
            {
            graphics3D::updateAll(3, 1);
            if ((lon == 0) && (DRV8834::time_until_turn_completion() < 50000)) { lon = 1; em = 0; lightOn(); }
            }
        graphics3D::cube3D.turn(1);
        if (lon == 0) { lightOn(); }
        while (em < 50) { graphics3D::updateAll(1); }


        // tumbling takes time so we can start it right away. 
        BasicMoves::tumble_start();

        // take the fourth picture
        ESPCAM::capture(_capture_image_cube, -1, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LX, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LY);
        lightOff();
        readFace(_imcam, face[3], rot[3]);

        while (BasicMoves::tumble_progress() < 1.0f) { graphics3D::updateAll(2); }
        graphics3D::cube3D.tumble();

        lightOn();
        em = 0; while (em < 50) { graphics3D::updateAll(1); } // wait 50ms for light to be fully on.

        // take the fifth picture
        ESPCAM::capture(_capture_image_cube, -1, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LX, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LY);
        lightOff();
        readFace(_imcam, face[4], rot[4]);

        BasicMoves::turn_start(2);
        lon = 0;
        while (BasicMoves::turn_progress() < 1.0f)
            {
            graphics3D::updateAll(3, 2);
            if ((lon == 0) && (DRV8834::time_until_turn_completion() < 50000)) { lon = 1; em = 0; lightOn(); }
            }
        graphics3D::cube3D.turn(2);
        if (lon == 0) { lightOn(); }
        while (em < 50) { graphics3D::updateAll(1); }

        // take the sixth picture
        ESPCAM::capture(_capture_image_cube, -1, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LX, ESPCAM_CAMERA_CONFIG_FRAMESIZE_LY);
        lightOff();
        readFace(_imcam, face[5], rot[5]);

        graphics3D::updateAll(1);

        DRV8834::enable(false);

        graphics3D::updateAll(1);

        if (findConfiguration())
            {
            DEBUG("readCubeAndFindConfiguration(): Success reading cube colors.\nCube configuration: " << cubeString());
            CubeOrientation::cube.reset('L', 'F', 'D', 'R', 'B', 'U');
            CubeState::cube.setState(cubeString());
            graphics3D::updateAll(1);
            return 0; 
            }
        graphics3D::updateAll(1);
        DEBUG("readCubeAndFindConfiguration(): **** ERROR : COULD NOT READ CUBE COLORS ****\n");
        return 2;
        }

    }


