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
// along with this code. If not, see  <http://www.gnu.org/licenses/>.


#include "SCommandResponder.h"

#include "Config.h"
#include "Hardware.h"
#include "Memory.h"
#include "debug.h"
#include "DRV8834.h"
#include "LedStrip.h"
#include "PowerCtrl.h"
#include "ESPCAM.h"
#include "SerialPacket.h"
#include "FBSensors.h"
#include "BasicMoves.h"
#include "panic.h"
#include "display.h"

#include <Arduino.h>
#include <TeensyThreads.h>



namespace SCommandResponder
{



#define SCOMMAND_ESPCAM_CAPTURE     1   // capture a screenshot. no parameters
#define SCOMMAND_ESPCAM_AEC         2   // change auto-exposure-compensation value.  0-1200 in value
#define SCOMMAND_LEDSTRIP_ALL       3   // turn on/off the ledstrips.
#define SCOMMAND_DRV8834_ENABLE     4   // turn on/off the step motor driver. 0-1 in val
#define SCOMMAND_DRV8834_STEP       5   // make some steps. DRV8834_CLOCKWISE_DIR/DRV8834_ANTICLOCKWISE_DIR in status and number of steps in value
#define SCOMMAND_DRV8834_TURN       6   // make some turn. DRV8834_CLOCKWISE_DIR/DRV8834_ANTICLOCKWISE_DIR in status and angle in fvalue
#define SCOMMAND_SYNC               7   // used for synchronization
#define SCOMMAND_VOLTAGE            8   // return curent voltage
#define SCOMMAND_TUMBLE             9   // tumble the cube (always disable after)
#define SCOMMAND_CLOSEGRIP          10  // close the grip (always disable after)
#define SCOMMAND_OPENGRIP           11  // open the grip (always disable after closing, send 1 for parking, 0 for normal)
#define SCOMMAND_ROTATE             12  // rotate the cube: DRV8834_CLOCKWISE_DIR/DRV8834_ANTICLOCKWISE_DIR or x2
#define SCOMMAND_ALIGNCRADDLE       13  // align the craddle
#define SCOMMAND_LEDSTRIP_SINGLE    14  // turn on/off a single led.


#define SCOMMAND_SYNC_INT1          1234567
#define SCOMMAND_SYNC_INT2          7654321


    struct SCommand
    {
        int command;
        int status;
        int value;
        float fvalue;
    };



    SLOWFUN void start()
    {
        INFO("\n\n **** SCommandResponder started **** \n\n")

        const size_t rx_buf_size = 1024;
        uint8_t rx_buf[rx_buf_size];

        SerialPacket::SerialPacket SP;
        SP.begin(&Serial, rx_buf, rx_buf_size);

        elapsedMillis emm;

        // buffer for an image    
        const int32_t CAMLX = 800;
        const int32_t CAMLY = 600;
        uint32_t* cambuf = (uint32_t*)extmem_malloc(CAMLX*CAMLY*sizeof(uint32_t)); /************ WARNING : memory never released. do not forget to free it if we ever leave this function */
        if (cambuf == nullptr) ABORT("Cannot allocate memory for camera buffer");
        tgx::Image<tgx::RGB32> imcam(cambuf,CAMLX,CAMLY); 

        while (1)
            {
            if (emm > 5000)
                {
                emm = 0;
                INFO("\n[SCommandResponder] voltage : \n"  << FBSensors::getVoltage() << "V\n")
                }
            threads.yield();

            if (SP.peek())
                {
                INFO("\nINPEEK....")
                SCommand sc;
                if (!SP.recv(sc)) ABORT("Wrong msg received" << SP.size());
                SP.next();

                switch (sc.command)
                    {
                    case SCOMMAND_ESPCAM_CAPTURE:
                        {
                        INFO("[SCommandResponder] IMAGE CAPTURE\n")
                        ESPCAM::capture_tgx(imcam);
                        tgx::Image<tgx::RGB565> screen = Display::image_main;
                        screen.copyFrom(imcam);
                        Display::update();
                        SP.send(CAMLX);
                        SP.send(CAMLY);
                        const int32_t ll = sizeof(uint32_t)*CAMLX;
                        for (int32_t j = 0; j < CAMLY; j++)
                            { // send line per line
                            SP.send((const char*)cambuf + (j*ll), ll);
                            }
                        break;
                        }
                    case SCOMMAND_SYNC:
                        {
                        INFO("[SCommandResponder] SYNC\n")
                        int sync[2];
                        sync[0] = SCOMMAND_SYNC_INT1;
                        sync[1] = SCOMMAND_SYNC_INT2;
                        SP.send((const char*)sync, 8);
                        break;
                        }
                    case SCOMMAND_VOLTAGE:
                        {
                        INFO("[SCommandResponder] VOLTAGE\n")
                        int v = (int)(1000 * FBSensors::getVoltage());
                        SP.send(v);
                        break;
                        }

                    case SCOMMAND_ESPCAM_AEC:
                        {
                        INFO("[SCommandResponder] AEC\n")
                        ESPCAM::setExposureValue(sc.value);
                        threads.delay(500); // wait for exposure to settle
                        SP.send(sc.command);
                        break;
                        }

                    case SCOMMAND_LEDSTRIP_ALL:
                        {
                        INFO("[SCommandResponder] LEDSTRIP ALL\n")
                        uint32_t v = (uint32_t)sc.value;
                        LedStrip::setAllLed(v & 255, (v >> 8) & 255, (v >> 16) & 255, (v >> 24) & 255);
                        LedStrip::show();
                        threads.delay(50);
                        SP.send(sc.command);
                        break;
                        }

                    case SCOMMAND_LEDSTRIP_SINGLE:
                        {
                        INFO("[SCommandResponder] LEDSTRIP SINGLE\n")
                        uint32_t v = (uint32_t)sc.value;
                        int index = sc.status;
                        LedStrip::setLed(index, v & 255, (v >> 8) & 255, (v >> 16) & 255, (v >> 24) & 255);
                        LedStrip::show();
                        threads.delay(50);
                        SP.send(sc.command);
                        break;
                        }

                    case SCOMMAND_DRV8834_ENABLE:
                        {
                        INFO("[SCommandResponder] DRV8834_ENABLE\n")
                        DRV8834::enable(sc.status);
                        threads.delay(50);
                        SP.send(sc.command);
                        break;
                        }

                    case SCOMMAND_DRV8834_STEP:
                        {
                        INFO("[SCommandResponder] STEP\n")
                        DRV8834::makeSteps(sc.status, sc.value);
                        threads.delay(50);
                        SP.send(sc.command);
                        break;
                        }

                    case SCOMMAND_DRV8834_TURN:
                        {
                        INFO("[SCommandResponder] TURN\n")
                        DRV8834::turn(sc.status, sc.fvalue, 0.0f);
                        DRV8834::waitTurnComplete();
                        threads.delay(50);
                        SP.send(sc.command);
                        break;
                        }


                    case SCOMMAND_TUMBLE:
                        {
                        INFO("[SCommandResponder] TUMBLE\n")
                        BasicMoves::tumble(sc.status);  // status = disable at end
                        threads.delay(50);
                        SP.send(sc.command);
                        break;
                        }

                    case SCOMMAND_CLOSEGRIP:
                        {
                        INFO("[SCommandResponder] CLOSEGRIP\n")
                        BasicMoves::closeGrip(sc.value, sc.status);  // status = disable at end, value = use previous pos
                        threads.delay(50);
                        SP.send(sc.command);
                        break;
                        }

                    case SCOMMAND_OPENGRIP:
                        {
                        INFO("[SCommandResponder] OPENGRIP\n")
                        BasicMoves::openGrip(sc.value, sc.status); // status = disable at end, value = park it
                        threads.delay(50);
                        SP.send(sc.command);
                        break;
                        }

                    case SCOMMAND_ROTATE:
                        {
                        INFO("[SCommandResponder] ROTATE\n")
                        BasicMoves::turn(sc.status); // status = turn
                        threads.delay(50);
                        SP.send(sc.command);
                        break;
                        }

                    case SCOMMAND_ALIGNCRADDLE:
                        {
                        INFO("[SCommandResponder] ALIGNCRADDLE\n")
                        DRV8834::alignCraddle();
                        threads.delay(50);
                        SP.send(sc.command);
                        break;
                        }

                    default:
                    {
                    INFO("[SCommandResponder] Unknown command " << sc.command << "\n")
                    SP.send(sc.command);
                    break;
                    }
                }
                    

            }
        }
    }


}
/** end of file */

