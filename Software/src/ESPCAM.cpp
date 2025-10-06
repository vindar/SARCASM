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


#include <Arduino.h>
#include <SPI.h>
#include <Audio.h>
#include <TeensyThreads.h>
#include "xorshift32.h"
#include "ESPCAM.h"

#include "SerialPacket.h"

#include "Memory.h"
#include "debug.h"
#include "panic.h"

#include "crc16_ccitt.hpp"




namespace ESPCAM
{


    /** structure for a command packet*/
    struct CommandPacket
        {
        int32_t val;
        uint8_t command;
        };


    /** Configuration list */
    static const ESPCAM::CommandPacket ESPCAM_CAMERA_CONFIG_ARRAY[21] =
        {
    // reset the camera
    { 0, ESPCAM_COMMAND_RESET },

    // taille et qualite de l'image
    { ESPCAM_CAMERA_CONFIG_FRAMESIZE, ESPCAM_COMMAND_FRAMESIZE },
    { ESPCAM_CAMERA_CONFIG_QUALITY, ESPCAM_COMMAND_QUALITY },

    // Désactivation des automatismes
    { ESPCAM_CAMERA_CONFIG_AWB, ESPCAM_COMMAND_AWB },
    { ESPCAM_CAMERA_CONFIG_AWB_GAIN, ESPCAM_COMMAND_AWB_GAIN },
    { ESPCAM_CAMERA_CONFIG_AEC, ESPCAM_COMMAND_AEC },
    { ESPCAM_CAMERA_CONFIG_AGC, ESPCAM_COMMAND_AGC },

    // reglage manuels
    { ESPCAM_CAMERA_CONFIG_WB_MODE, ESPCAM_COMMAND_WB_MODE },
    { ESPCAM_CAMERA_CONFIG_AEC_VALUE, ESPCAM_COMMAND_AEC_VALUE },
    { ESPCAM_CAMERA_CONFIG_AGC_GAIN, ESPCAM_COMMAND_AGC_GAIN },

    { ESPCAM_CAMERA_CONFIG_GAINCEILING, ESPCAM_COMMAND_GAINCEILING },
    { ESPCAM_CAMERA_CONFIG_AEC2, ESPCAM_COMMAND_AEC2 },
    { ESPCAM_CAMERA_CONFIG_AE_LEVEL, ESPCAM_COMMAND_AE_LEVEL },

    // calibration et corrections
    { ESPCAM_CAMERA_CONFIG_BPC, ESPCAM_COMMAND_BPC },
    { ESPCAM_CAMERA_CONFIG_WPC, ESPCAM_COMMAND_WPC },
    { ESPCAM_CAMERA_CONFIG_RAW_GMA, ESPCAM_COMMAND_RAW_GMA },
    { ESPCAM_CAMERA_CONFIG_LENC, ESPCAM_COMMAND_LENC },
    //{ ESPCAM_CAMERA_CONFIG_DENOISE,ESPCAM_COMMAND_DENOISE },

    // apparence
    { ESPCAM_CAMERA_CONFIG_BRIGHTNESS, ESPCAM_COMMAND_BRIGHTNESS },
    { ESPCAM_CAMERA_CONFIG_CONTRAST, ESPCAM_COMMAND_CONSTRAST },
    { ESPCAM_CAMERA_CONFIG_SATURATION, ESPCAM_COMMAND_SATURATION },
    //{ ESPCAM_CAMERA_CONFIG_SHARPNESS,  ESPCAM_COMMAND_SHARPNESS },

    // orientation et format
    //{ ESPCAM_CAMERA_CONFIG_SPECIAL_EFFECT, ESPCAM_COMMAND_SPECIAL_EFFECT },
    //{ ESPCAM_CAMERA_CONFIG_HMIRROR, ESPCAM_COMMAND_HMIRROR },
    //{ ESPCAM_CAMERA_CONFIG_VFLIP, ESPCAM_COMMAND_VFLIP },
    //{ ESPCAM_CAMERA_CONFIG_COLORBAR, ESPCAM_COMMAND_COLORBAR },
    { ESPCAM_CAMERA_CONFIG_DCW, ESPCAM_COMMAND_DCW },
        };


    /** structure used to indicate the lenght of a jpeg buffer and its checksum */
    struct DataSnapshot
        {
        int32_t length;
        uint32_t crc16;
        };


    // the Serial Packet object. 
    SerialPacket::SerialPacket _espcam_sp;


    DMAChannel*     _prx = nullptr;         // the dma channel object use for dma transfer. 

    volatile int    _off_dma1;              // offset of the content in buffer dma1
    volatile int    _off_dma2;              // offset of the content in buffer dma2
    volatile int    _nb_read;               // number of bytes already read
    volatile int    _len_transfer;          // total length of the transfer

    volatile int    _transfer_ind;          // index of the buffer being used for transfer (0 if disabled, 1 or 2)
    volatile int    _transfer_off;          // offset of the buffer being transfered
    volatile int    _next_off;              // offset of the next chunk to query (already successfully queried [0,_next_off[).
    
    elapsedMicros	_emdma;                 // time since spi transfer was requested

    CRC16           _chk16;                 // checksum for the data transfer

    inline bool _isObsolete1() 
        { 
        return (_nb_read >= _off_dma1 + ESPCAM_SPI_CHUNK_SIZE); 
        }

    inline bool _isObsolete2() 
        { 
        return (_nb_read >= _off_dma2 + ESPCAM_SPI_CHUNK_SIZE); 
        }

    inline int _isin1() 
        { 
        const int o = _nb_read - _off_dma1;
        return (((o < 0) || (o >= ESPCAM_SPI_CHUNK_SIZE)) ? -1 : o);
        }

    inline int _isin2() 
        { 
        const int o = _nb_read - _off_dma2;
        return (((o < 0) || (o >= ESPCAM_SPI_CHUNK_SIZE)) ? -1 : o);
        }

    




    /**
    * Check that a jpeg file looks correct.
    * if lx or/and ly are specified, check also that the size of the image matches (lx,ly).
    * return 1 if ok and -error code otherwise.
    **/
    int jpeg_check(const uint8_t* buf, const int len, const int lx = 0, const int ly = 0)
       {
        int w = 0;
        int h = 0;
        if ((!buf)||(len <= 0)) return -1;
        if (buf[0] != 0xFF) return -2;
        if (buf[1] != 0xD8) return -3;
        int off = 2;

        while (1)
            {
            if (off > len - 2) return -4; // cannot read a marker
            const int c = (((int)buf[off]) * 256) + ((int)buf[off + 1]);
            off += 2;

            switch (c)
                {

            case 0xFFC0: // start of frame (baseline dct)
                {
                if (off > len - 2) return -5;
                int l = (((int)buf[off]) * 256) + ((int)buf[off + 1]);
                if (l < 7) return -6;
                h = (((int)buf[off + 3]) * 256) + ((int)buf[off + 4]);  // read the image dimensions
                w = (((int)buf[off + 5]) * 256) + ((int)buf[off + 6]);  //
                off += l;
                break;
                }

            case 0xFFC2: // start of frame (progressive dct)
                return -7; // forbidden here

            case 0xFFDA: // start of scan
                {
                int l = (((int)buf[off]) * 256) + ((int)buf[off + 1]);
                if (l != 12) return -8;
                off += l;
                while (1)
                    {
                    if (off >= len - 1) return -9;
                    if (buf[off] == 0xFF)
                        {
                        const uint8_t c = buf[off + 1];
                        if ((c == 0) || ((c >= 0xD0) && (c <= 0xD7)))
                            {
                            off++; // skip 0xFF00 and restartN
                            }
                        else
                            {
                            if (c != 0xFF) break; // marker found
                            // stuffing with 0xFF
                            off++; 
                            while (((off + 1 < len) && (buf[off + 1] == 0xFF))) { off++; }
                            if (off + 1 >= len) return -10;
                            break; // marker found
                            }
                        }
                     off++;
                    }
                break;
                }
            case 0xFFC4: // define huffman table
            case 0xFFDB: // define quantization tables
            case 0xFFDD: // define restart interval
            case 0xFFFE: // comment

            case 0xFFE0: // app specific
            case 0xFFE1: //
            case 0xFFE2: //
            case 0xFFE3: //
            case 0xFFE4: //
            case 0xFFE5: //
            case 0xFFE6: //
            case 0xFFE7: //
            case 0xFFE8: //
            case 0xFFE9: //
            case 0xFFEA: //
            case 0xFFEB: //
            case 0xFFEC: //
            case 0xFFED: //
            case 0xFFEE: //
            case 0xFFEF: //

                {
                if (off > len - 2) return -11;
                const int l = (((int)buf[off]) * 256) + ((int)buf[off + 1]);
                if (l < 2) return -12;
                off += l;
                }

            case 0xFFD0:
            case 0xFFD1:
            case 0xFFD2:
            case 0xFFD3:
            case 0xFFD4:
            case 0xFFD5:
            case 0xFFD6:
            case 0xFFD7:

                break;

            case 0xFFD9: // end of image
                if (off == len)
                    {
                    if ((lx > 0) && (w != lx)) return -13;
                    if ((ly > 0) && (h != ly)) return -14;
                    return 1; // OK !
                    }
                return -15;

            default:
                return -16;
                }
            }
        }





    SLOWFUN void _beginPowerCycling()
        {
        DEBUG("**WARNING** BEGINNING POPWER CYCLING FOR ESPCAM")
        digitalWrite(PIN_ESPCAM_POWER_CTRL, LOW);
        ESPCAM_Serial.flush();
        ESPCAM_Serial.end();
        }


    SLOWFUN void _endPowerCycling()
        {
        DEBUG("**WARNING** END POWER CYCLING FOR ESPCAM")
        // completely flush any pending data
        while (_espcam_sp.peek() > 0) { _espcam_sp.next(); }
        ESPCAM_Serial.begin(ESPCAM_DEFAULT_SERIAL_SPEED);
        ESPCAM_Serial.addMemoryForRead(Memory::buffer_rx_serial_espcam, ESPCAM_RX_buffer_size);
        ESPCAM_Serial.flush();
        while (_espcam_sp.peek() > 0) { _espcam_sp.next(); }
        __DMB();   // prevent reordering
        digitalWrite(PIN_ESPCAM_POWER_CTRL, HIGH); // power on again. 
        }


    // 0 = defore init()
    // 1 = init() called, waiting for status
    // 2 = waiting for powercycling
    // 3 = ready for init
    // 4 = postinit() called
    volatile int _inited_espcam = 0;
    elapsedMillis _powercycling_em; 


    SLOWFUN void checkPowerCycling()
        {
        if ((_inited_espcam == 1) && (_powercycling_em > ESPCAM_POWERCYCLING_CHECK_MS))
            { // init() called, waiting for status
            _powercycling_em = 0;
            if (_espcam_sp.peek() == 0) return; // no data waiting
            int greeting;
            if (!_espcam_sp.recv(greeting)) { _beginPowerCycling(); _inited_espcam = 2; return; }
            if (greeting != ESPCAM_GREETING_CODE) { _beginPowerCycling(); _inited_espcam = 2; return; }
            DEBUG("ESPCAM IS READY FOR USE")
            _inited_espcam = 3;
            return;
            }
        if ((_inited_espcam == 2)&& (_powercycling_em > ESPCAM_POWERCYCLING_DELAY_MS))
            { //
            _endPowerCycling();
            _powercycling_em = 0;
            _inited_espcam = 1;
            }
        return;
        }



    void init()
        {
        static int8_t _init_done = 0;
        if (_init_done) return;
        _init_done = 1;

        debug::init(); // just in case
        DEBUG("ESPCAM::init() started")

        // configure serial and SerialPacket
        ESPCAM_Serial.begin(ESPCAM_DEFAULT_SERIAL_SPEED);
        ESPCAM_Serial.addMemoryForRead(Memory::buffer_rx_serial_espcam, ESPCAM_RX_buffer_size);
        _espcam_sp.begin(&Serial6, Memory::buffer_serialpacket_espcam, ESPCAM_SerialPacket_buffer_size);

        // configure power pins
        digitalWrite(PIN_ESPCAM_FLASH, HIGH);
        pinMode(PIN_ESPCAM_FLASH, OUTPUT);
        digitalWrite(PIN_ESPCAM_FLASH, HIGH);

        digitalWrite(PIN_ESPCAM_POWER_CTRL, LOW);
        pinMode(PIN_ESPCAM_POWER_CTRL, OUTPUT);
        digitalWrite(PIN_ESPCAM_POWER_CTRL, LOW);

        digitalWrite(PIN_ESPCAM_EXTRA, LOW);
        pinMode(PIN_ESPCAM_EXTRA, OUTPUT);
        digitalWrite(PIN_ESPCAM_EXTRA, LOW);

        // create dma channel object for spi transfer
        _prx = new DMAChannel(false);
        _prx->begin(false);
        _prx->source((uint8_t&)LPSPI4_RDR);
        _prx->triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI4_RX);

        // turn on the module
        threads.delay(5);
        digitalWrite(PIN_ESPCAM_POWER_CTRL, HIGH);
        threads.delay(5);
//        digitalWrite(PIN_ESPCAM_EXTRA, HIGH);
//      threads.delay(5);

        __DMB();
        _inited_espcam = 1; // started
        DEBUG("ESPCAM::init() completed.\n")
        }



    void _rxISR();

 
    void _enableSPISlave()
        {
        LPSPI4_CR &= ~LPSPI_CR_MEN;         //Disable SPI module
        LPSPI4_CFGR1 &= ~LPSPI_CFGR1_MASTER;// slave mode
        LPSPI4_CR = LPSPI_CR_RST;           //Master Logic reset! (Control Register => Software Reset)
        LPSPI4_CR &= ~LPSPI_CR_RST;         //Master Logic reset! (Control Register => Software Reset)
        LPSPI4_TCR = LPSPI_TCR_FRAMESZ(7);	//8Bit Mode
        LPSPI4_DER = LPSPI_DER_RDDE;        //RX DMA Request Enable
        LPSPI4_CR |= LPSPI_CR_MEN;          //Enable SPI Module!
        //NVIC_SET_PRIORITY(IRQ_LPSPI4, 0);   //Highest priority, useful ? 
        }


    void _disableSPISlave()
        {
        LPSPI4_CR &= ~LPSPI_CR_MEN;         //Disable SPI module
        }


    inline void _disableDMATransfer()
        {
        _prx->clearInterrupt();
        _prx->clearComplete();
        _prx->clearError();
        _prx->disable();
        }


    void _enableDMATransfer(int index)
        {
        _prx->attachInterrupt(_rxISR);
        _prx->interruptAtCompletion();
        _prx->disableOnCompletion();
        _prx->destinationBuffer((volatile signed char *)((index == 1) ? Memory::buffer_espcam_dma1 : Memory::buffer_espcam_dma2), ESPCAM_SPI_CHUNK_SIZE + ESPCAM_SPI_CHUNK_TRAILING);
        //NVIC_SET_PRIORITY(IRQ_DMA_CH0 + _prx->channel, 16); // Set a high priority for DMA interrupt, is this really useful ? 
        _prx->enable();
        _enableSPISlave();
        }


    /* called when transfer is complete */
    void _rxISR()
        {
        _disableDMATransfer();

        //elapsedMicros erm = 0;
        //while (erm < 500) {}
        //ESPCAM_DEBUG("COMPLETED TRANSFER :  index=" << _transfer_ind << "  off=" << _next_off);
        
        void * b = ((_transfer_ind == 1) ? Memory::buffer_espcam_dma1 : Memory::buffer_espcam_dma2);
        if ((uint32_t)b >= 0x20200000u)
            {
            arm_dcache_flush(b, ESPCAM_SPI_CHUNK_SIZE + ESPCAM_SPI_CHUNK_TRAILING); // clear cache if buffer is in DMAMEM
            }
       
        if (_transfer_ind == 1) _off_dma1 = _transfer_off; else _off_dma2 = _transfer_off;
        _next_off = _transfer_off + ESPCAM_SPI_CHUNK_SIZE;
        _transfer_ind = 0;
        }


    void _requestbuffer(int index)
        {
        //ESPCAM_DEBUG("REQUEST TRANSFER : index=" << index << "  off=" << _next_off);
        _transfer_ind = index;
        _transfer_off = _next_off;
        if (_next_off >= _len_transfer)
           {  // no need to do actual transfer in this case
            //ESPCAM_DEBUG("No need: fake transfer");
            if (index == 1) _off_dma1 = _next_off; else _off_dma2 = _next_off;
           _next_off = _transfer_off + ESPCAM_SPI_CHUNK_SIZE;
           _transfer_ind = 0;
            return;
           }
        _enableDMATransfer(index);
        _espcam_sp.send(_next_off);
        _emdma = 0;
        }

 
    size_t _tjpgdec_input_fun(JDEC * jd, uint8_t * buff, size_t nbyte)
        {
        // reduce nbyte if we go over the end. 
        const size_t rem = _len_transfer - _nb_read;
        if (nbyte > rem) nbyte = rem;
        if (nbyte == 0) return 0;
        //ESPCAM_DEBUG("input fun : nb_read=" << _nb_read << " nbyte=" << nbyte)
        // loop
        while (1)
            {          
            if (_transfer_ind == 0)
                { // no transfer in progress: start one if possible
                if (_isObsolete1())
                    {
                    _requestbuffer(1);
                    }
                else if (_isObsolete2())
                    {
                    _requestbuffer(2);
                    }                        
                } 
            if ((_transfer_ind > 0) && (((int)_emdma) > ESPCAM_DMA_RETRY_US))
                { 
                _espcam_sp.send(-1); // make sure espcam has finished using the spi bus
                noInterrupts();
                if (_transfer_ind > 0)
                    { // a transfer is hanging: stop and restart it.
                    //ESPCAM_DEBUG("**** HANGING **** ind=" << _transfer_ind << " off=" << _next_off)
                    _disableSPISlave();
                    _disableDMATransfer();
                    _enableSPISlave();
                    interrupts();
                    _requestbuffer(_transfer_ind); // restart
                    }
                else
                    {
                    interrupts();
                    }                
                }
            const int o1 = _isin1();
            if (o1 >= 0)
                { // ok, read from buffer 1
                //ESPCAM_DEBUG("input_end 1")
                const uint8_t* bb = ((uint8_t*)Memory::buffer_espcam_dma1) + o1;                
                _chk16.ccitt_upd(bb, nbyte);
                if (buff) { memmove(buff, bb, nbyte); }
                _nb_read += nbyte;
                return nbyte;
                }
            const int o2 = _isin2();
            if (o2 >= 0)
                { // ok, read from buffer 2
                //ESPCAM_DEBUG("input_end 2")
                const uint8_t* bb = ((uint8_t*)Memory::buffer_espcam_dma2) + o2;
                _chk16.ccitt_upd(bb, nbyte);
                if (buff) memmove(buff, bb, nbyte);
                _nb_read += nbyte;
                return nbyte;
                }            
            threads.yield(); // data not ready, wait a bit. 
            }
        }


    void _postinit();






    tgx::Image<tgx::RGB32> _tgx_im; 

    int _output_fun_capture_tgx(JDEC* jdec, void* bitmap, JRECT* rect)
        {
        uint8_t* p = (uint8_t*)bitmap;
        for (int j = rect->top; j <= rect->bottom; j++)
            {
            for (int i = rect->left; i <= rect->right; i++)
                {
                _tgx_im(i, j) = tgx::RGB32(p[0], p[1], p[2]);
                p += 3;
                }
            }
        return 1;
        }

    SLOWFUN void capture_tgx(tgx::Image<tgx::RGB32>& im, int nb_retry)
        {
        _tgx_im = im;
        capture(_output_fun_capture_tgx, nb_retry, im.lx(), im.ly());
        }



    tgx::iVec2 capture(TJPGDEC_OUTPUT_FUN output_fun, int nb_retry, int lx, int ly)
        {
        _postinit();
        while ((--nb_retry) != -1)
            {
            // send command for spi transfer and get the size of the image. 
            sendCommand(ESPCAM_COMMAND_SPI_SNAPSHOT, 0);        
            _espcam_sp.wait();

            DataSnapshot ds;
            if (!_espcam_sp.recv(ds)) { ABORT("wrong size for length packet"); }
            _espcam_sp.next();
            _len_transfer = ds.length;
            if ((_len_transfer < 1024) || (_len_transfer > 128 * 1024))
                { // wrong size for a valid image
                _espcam_sp.send(-2); // quit spi transfer mode
                threads.delay(20); // wait for another image
                continue;
                }

            // ok, we try 3 times to tranfer the image
            for (int irr = 0; irr < 3; irr++)
                { 
                _off_dma1 = -ESPCAM_SPI_CHUNK_SIZE; // mark as obsolete.
                _off_dma2 = -ESPCAM_SPI_CHUNK_SIZE; //
                _nb_read = 0;                       // nothing as been read yet
                _transfer_ind = 0;                  // no ongoing transfer
                _next_off = 0;                      // start from the begining 
                _transfer_off = 0;                  // 
                _chk16.reset();                     // reset checksum

                // enable spi 
                _enableSPISlave();

                // request the first chunk
                _requestbuffer(1);

                // jpeg decompressor object and workspace (on the stack)
                JDEC jdec;

                // prepare decompression 
                JRESULT res = jd_prepare(&jdec, _tjpgdec_input_fun, Memory::dma_shared.A.buffer_tjpgd_work, ESPCAM_TJPGD_WORK_SIZE, nullptr);
                if (res != JDR_OK)
                    { // error in preparation
                    _transfer_ind = 0;
                    _disableDMATransfer();
                    _disableSPISlave();                    
                    ESPCAM_DEBUG("JD_PREPARE ERROR " << res)
                    threads.delay(1);
                    continue; // try again
                    }

                const int ilx = jdec.width;
                const int ily = jdec.height;

                if ((lx > 0) && (ilx != lx))
                    {
                    ABORT("capture: wrong image width " << ilx << "\nexpected: " << lx);
                    }
                if ((ly > 0) && (ily != ly))
                    {
                    ABORT("capture: wrong image height " << ily << "\nexpected: " << ly);
                    }

                // decompress
                res = jd_decomp(&jdec, output_fun, 0);

                _transfer_ind = 0;
                _disableDMATransfer();
                _disableSPISlave();
                if (res != JDR_OK)
                    {
                    ESPCAM_DEBUG("JD_DECOMP ERROR " << res)
                    threads.delay(1);
                    continue;
                    }

                uint32_t crcr = ((uint16_t)_chk16); // checksum of transfer received.
                if (crcr != ds.crc16)
                    {
                    ESPCAM_DEBUG("JD_DECOMP CHECKSUM ERROR")
                    threads.delay(1);
                    continue;
                    }

                // ok: image correctly downloaded !
                _espcam_sp.send(-2); // quit spi transfer mode 
                return tgx::iVec2(jdec.width, jdec.height);
                }
            _espcam_sp.send(-2); // quit spi transfer mode 
            threads.delay(20); // wait for another image
            }
        ABORT("ESPCAM::capture() : Too many retry")
        return tgx::iVec2(0,0);
        }



    int captureraw(uint8_t* startbuffer, int sizebuf, int nb_retry)
        {
        _postinit();
        while ((--nb_retry) != -1)
            {
            // send command for spi transfer and get the size of the image. 
            sendCommand(ESPCAM_COMMAND_SPI_SNAPSHOT, 0);
            _espcam_sp.wait();
            DataSnapshot ds;
            if (!_espcam_sp.recv(ds))
                {
                // this should never happen so we abort
                ABORT("wrong size for length packet");
                }
            _espcam_sp.next();
            _len_transfer = (int)ds.length;
            if (_len_transfer < 100)
                {
                //_espcam_sp.send(-2); // quit spi transfer mode
                ABORT("wrong image length :" << _len_transfer);
                }
            if (_len_transfer >= sizebuf)
                { 
                //_espcam_sp.send(-2); // quit spi transfer mode
                ABORT("buffer too small: " << sizebuf << "\nneeded: " << _len_transfer);
                }

            for (int nbt = 0; nbt < 3; nbt++) // retry SPI transfer 3 times before switching to serial
                {
                uint8_t* buffer = startbuffer;
                _off_dma1 = -ESPCAM_SPI_CHUNK_SIZE; // mark as obsolete.
                _off_dma2 = -ESPCAM_SPI_CHUNK_SIZE; //
                _nb_read = 0;                       // nothing as been read yet
                _transfer_ind = 0;                  // no ongoing transfer
                _next_off = 0;                      // start from the begining 
                _transfer_off = 0;                  // 
                _chk16.reset();                     // reset checksum
                
                // enable spi 
                _enableSPISlave();
                // request the first chunk
                _requestbuffer(1);
                int rlen = _len_transfer;
                //memset(startbuffer, 0xFF, ds.length);
                while (rlen >= 1024)
                    {
                    _tjpgdec_input_fun(nullptr, buffer, 1024);
                    buffer += 1024;
                    rlen -= 1024;
                    }
                if (rlen > 0)
                    {
                    _tjpgdec_input_fun(nullptr, buffer, rlen);
                    }
                _transfer_ind = 0;
                _disableDMATransfer();
                _disableSPISlave();

                uint32_t crcr = ((uint16_t)_chk16); // checksum of transfer received.
                if (crcr == ds.crc16)
                    { // ok, good crc
                    const int okjpeg = jpeg_check(startbuffer, _len_transfer);
                    if (okjpeg == 1)
                        { // all good !
                        _espcam_sp.send(-2); // quit spi transfer mode
                        return _len_transfer;
                        }
                    // wrong jpeg format but good crc, this is very strange
                    ESPCAM_DEBUG(" *****  ESPCAM::captureraw() : good CRC but invalid jpeg file... Strange !!!! ******");
                    } 
                else
                    {
                    ESPCAM_DEBUG(" ESPCAM::captureraw() : SPI transfer error, wrong CRC received=" << crcr << " expected=" << ds.crc16);
                    //memset(startbuffer, 0xFF, ds.length);
                    }
                threads.delay(1);
                }
            // go to serial transfer instead.
            ESPCAM_DEBUG("ESPCAM::captureraw() : Too many error, trying serial transfer instead...");
            _espcam_sp.send(-3); // go to serial tranfer instead. 
            int rlen = ds.length;
            uint8_t* buf = startbuffer;
            while (rlen > 0)
                {
                _espcam_sp.wait();
                const int l = _espcam_sp.size();
                memmove(buf, _espcam_sp.recv_buffer(), l);
                _espcam_sp.next();
                buf += l;
                rlen -= l;
                }
            if (rlen != 0) ABORT("Error during serial transfer of frame : " << rlen);
            CRC16 crc16;
            uint32_t crcr = (uint32_t)crc16.ccitt(startbuffer, ds.length);
            if (crcr != ds.crc16)
                {
                ESPCAM_DEBUG(" *****  ESPCAM::captureraw() : Wrong CRC by Serial ! ******");                
                }
            else
                { 
                const int okjpeg = jpeg_check(startbuffer, _len_transfer);
                if (okjpeg == 1) return _len_transfer;
                ESPCAM_DEBUG(" *****  ESPCAM::captureraw() : Serial CRC ok but invalid jpeg file. Strange ! ******");
                }
            }    
        ABORT("ESPCAM::captureraw() : Too many retry")
        return -1;
        }

    
   void discardframe()
       {
       const int32_t chkcode = 123;
       sendCommand(ESPCAM_COMMAND_DISCARD_FRAME, chkcode);
       //_espcam_sp.wait();
       //int32_t br;
       //if (!_espcam_sp.recv(br)) ABORT("wrong size for received msg.");
       //if (br != chkcode) ABORT("wrong code received");
       //_espcam_sp.next();
       }



    void _postinit()
        {
        noInterrupts();
        const int fi = _inited_espcam;
        _inited_espcam = 4;
        interrupts();
        __DMB();
        if (fi == 4) return; // already done
        if (fi == 0) { ABORT("postinit() before init()"); }
        if (fi == 2) { ABORT("postinit() during powercycling"); }
        DEBUG("ESPCAM::_postinit() started.\n")

        // receive greeting
        const int nbm = _espcam_sp.wait(1000);
        if (nbm <= 0)
            {
            ABORT("ESPCAM : no message received from camera.");
            }
        
        int greeting;
        if (!_espcam_sp.recv(greeting)) { ABORT("wrong greeting msg size"); }
        if (greeting != ESPCAM_GREETING_CODE)
            { // something is wrong
            if (greeting != ESPCAM_MALFUNCTION_CODE)
                { // this should never happen
                ABORT("wrong greeting code received");
                } 
            else
                { // here we may try to power cycle the espcam module
                // TODO implement power cycling
                ABORT("ESPCAM malfunction. Powercycling required");
                }
            }

        while (_espcam_sp.peek() > 0) { _espcam_sp.next(); }

        // ping the espcam 
        int s = rng::rand(1000000000);
        sendCommand(ESPCAM_COMMAND_PING, s);
        int r;
        _espcam_sp.wait();

        if (!_espcam_sp.recv(r)) ABORT("wrong size answered for ping msg.");
        if (r != s) ABORT("wrong ping value received");
        _espcam_sp.next();
        
        // configure the PSI module 
        SPI.begin();
        SPI.setCS(PIN_ESPCAM_CS);
        _disableSPISlave();

        // change the serial speed
        setSerialSpeed(ESPCAM_SERIAL_SPEED);

        // set the spi speed
        setSpiSpeed(ESPCAM_SPI_SPEED);

        // set the spi chunk size
        setSpiChunkSize(ESPCAM_SPI_CHUNK_SIZE);

        // finally, we configure the camera by sending all the config commands
        // ALREADY CONFIGURED INSDE ESPCAM
        /*
        const int nbc = sizeof(ESPCAM_CAMERA_CONFIG_ARRAY) / sizeof(ESPCAM::CommandPacket);
        for (int i = 0; i < nbc; i++)
            { // execute all config commands 
            DEBUG("Sending " << i << " with command " << ESPCAM_CAMERA_CONFIG_ARRAY[i].command << " and value " << ESPCAM_CAMERA_CONFIG_ARRAY[i].command  <<"\n")
            _espcam_sp.send(ESPCAM_CAMERA_CONFIG_ARRAY[i]);
            }      
        */

        //for (int i = 0; i < 3; i++) { discardframe(); } // discard the first 3 frames
        
        DEBUG("ESPCAM::_postinit() completed.\n")
        }




    FrameStat frameStats()
        {
        _postinit();
        CommandPacket cp;
        cp.command = ESPCAM_COMMAND_FRAME_STATS;
        _espcam_sp.send(cp);
        _espcam_sp.wait();
        FrameStat fs;
        if (!_espcam_sp.recv(fs)) ABORT("wrong size for frameSize packet");
        _espcam_sp.next();
        return fs;
        }


    void sendCommand(espcam_commands command, int value)
        {
        _postinit();
        CommandPacket cp;
        cp.command = command;
        cp.val = value;
        _espcam_sp.send(cp);
        }



	void setSerialSpeed(int baudrate)
		{
		sendCommand(ESPCAM_COMMAND_SERIAL_SPEED, baudrate);
		_espcam_sp.wait();
		int br;
		if (!_espcam_sp.recv(br)) ABORT("wrong size for received msg.");
		if (br != baudrate) ABORT("wrong baudrate received");
        _espcam_sp.next();
        
		// ok, we can change serial baudrate now
		ESPCAM_Serial.flush();
		ESPCAM_Serial.begin(baudrate);
		ESPCAM_Serial.addMemoryForRead(Memory::buffer_rx_serial_espcam, ESPCAM_RX_buffer_size);
        ESPCAM_Serial.flush();
        threads.delay(10);
        DEBUG("ESPCAM Serial speed set: " << baudrate)
		}



}



/** end of file */

;