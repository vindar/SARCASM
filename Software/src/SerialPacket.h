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

#include <Arduino.h>
#include <TeensyThreads.h>

#include "Config.h"
#include "debug.h"
#include "panic.h"

#include "crc16_ccitt.hpp"




// decide whether debug message are send to debug serial port. 
#define ENABLE_SERIAL_PACKET_DEBUG 1


#if ENABLE_SERIAL_PACKET_DEBUG
#define SERIAL_PACKET_DEBUG(V) { DEBUG(V); }
#else 
#define SERIAL_PACKET_DEBUG(V) 
#endif 


#define SERIAL_PACKET_TAGSTART (0b10100110)
#define SERIAL_PACKET_ESCAPE   (0b11010010)

#define SERIAL_PACKET_RETRY_MS 50

#define SERIAL_PACKET_STATE_IDLE            0
#define SERIAL_PACKET_STATE_ID              1
#define SERIAL_PACKET_STATE_SIZE1           2
#define SERIAL_PACKET_STATE_SIZE2           3
#define SERIAL_PACKET_STATE_SIZE3           4
#define SERIAL_PACKET_STATE_PAYLOAD_FIRST   5
#define SERIAL_PACKET_STATE_PAYLOAD_NEXT    6
#define SERIAL_PACKET_STATE_CRC1            7
#define SERIAL_PACKET_STATE_CRC2            8


namespace SerialPacket
{


    /**
    * Class for reliable communication via a serial port. 
    * 
    * (1) The object must first be attached to a Serial port and provided with a receive buffer. 
    * (2) The poll() method must be called regularly to query incomming packets (possibly from within an interrupt).
    * 
    * - Messages are sent as packets whose size can range from 1 to 65535 bytes.      
    * - Checksum and receive acknowledgment are used to insure that messages are correctly transmitted. 
    **/
    class SerialPacket
        {

        public:


            /**
            * Ctor, does nothing
            **/
            SerialPacket() : _busy(0) { }


            /**
            * Initialise the object and attach it to a serial port and a receive buffer. 
            * 
            * - The serial port must already be opened (with .begin()). 
            * - The receive buffer must be larger than the largest packet that is transmitted. 
            * 
            * Note: it can also be useful to increase the Serial internal read buffer size
            *       with the command Serial.addMemoryForRead(), especially when receiving large
            *       packets at high baud rates. 
            **/
            SLOWFUN void begin(Stream* serial, uint8_t* buffer_recv, int len);


            /**
            * Send a packet/message. 
            * 
            * If confirm = true, wait for acknowledgment before returning (recommended). In this case
            * resend the msg after retry_ms milliseconds if no ack received.
            **/
            SLOWFUN void send(const char* buffer, uint16_t len, bool confirm = true, int retry_ms = SERIAL_PACKET_RETRY_MS);


            /**
            * Send a message/packet (template version)
            * 
            * If confirm = true, wait for acknowledgment before returning (recommended).
            **/
            template<typename T> void send(const T& msg)
                {
                send((const char *)(&msg), sizeof(msg));
                }


            /**
            * Wait until at least one message is available in the receive queue. 
            * 
            * - if timeout_ms >= 0, return after timeout_ms milliseconds if no msg available. 
            * - if timeout < 0, wait forever until a msg is received. 
            * 
            * Return the number of messages available.
            **/
            SLOWFUN int wait(int timeout_ms = -1);


            /**
            * Return immediately with the number of messages available in the receive queue.
            **/
            int peek() {  return wait(0); }


            /**
            * Return the length of the current message in the receive queue.
            * 
            * Return 0 is none available.
            **/
            SLOWFUN int size();
               

            /**
            * Return a pointer to the begining of the current msg on the receive queue. 
            * 
            * Returns nullptr if the receive queue is empty.
            **/
            SLOWFUN const char* recv_buffer();
                

            /**
            * Copy the current message into msg of type T.
            * Return true on success and false if the current message has the wrong size. 
            * 
            * if strict_size_comparison = true, then message and type must have the same
            * size. Otherwise, msg can be longer than sizeof(T). 
            **/
            template<typename T> bool recv(T& msg, bool strict_size_comparison = true)
                {
                const int l = size();
                if ((l < ((int)sizeof(msg)))||(strict_size_comparison && (l > ((int)sizeof(msg)))))
                    {
                    SERIAL_PACKET_DEBUG("recv() WRONG SIZE : " << size() << sizeof(msg));
                    return false;
                    }
                memcpy(&msg, recv_buffer(), sizeof(msg));
                return true; 
                }


            /** 
            *  Remove the current message from the receive queue (and go the the next one if available).
            **/
            SLOWFUN void next();
                

            /**
            * Check the serial port receive buffer to check for incoming message. 
            * 
            * This method should be called as often as possible. 
            * 
            * Note: This method can be called from an interrupt (e.g. an intervalTimer object) but
            *       one should make sure that the priority is not set too high (lower than the Serial 
            *       interrupt at the very least)...
            **/
            void poll()
                {
                if (!_mutex_start()) return; // cannot poll right now. 
                _poll();
                _mutex_stop();
                }





        private:

            /**********************************************************
            * Welcome to my magic private world !
            ***********************************************************/


            /**
            * The main poll method (without mutex guards)
            **/
            void _poll();


            /**
            * Read a char from the stream (if available). 
            * - return -1 if no char available. 
            * - return -2 if the start of frame tag was received. 
            * - otherwise, return the char value in [0,255]
            **/
            SLOWFUN int _read();
               

            /**
            * Return the crc value for the current ongoing frame. 
            **/
            uint16_t _get_read_crc()
                {
                const uint8_t b = 0;
                return _crc_read.ccitt_upd(&b, 1);
                }


            /**
            * Send a start frame tag (reset the crc).
            **/
            void _writeFrameStart()
                {
                const uint8_t b = SERIAL_PACKET_TAGSTART;
                _serial->write(b);
                _crc_write.ccitt(&b, 1);
                }


            /**
            * Write a char into the stream
            **/
            SLOWFUN void _write(uint8_t c);


            /**
            * Write a buffer into the stream
            **/
            SLOWFUN void _write(const uint8_t* buffer, int len);
            

            /**
            * Write the crc into the stream
            **/
            uint16_t _get_write_crc()
                {
                const uint8_t b = 0;
                return _crc_write.ccitt_upd(&b, 1);
                }


            /**
            * Send an ack. packet with a given id
            **/
            SLOWFUN void _send_ack(uint8_t id);


            /**
            * write a message size in the buffer
            **/
            void _write_msg_buf_size(int off, int size)
                {
                _buffer[off]     = (uint8_t)(size & 255);
                _buffer[off + 1] = (uint8_t)(size / 256);
                }


            /**
            * read a message size in the buffer
            **/
            int _read_msg_buf_size(int off) const
                {
                return (((uint8_t)_buffer[off]) + 256 * ((uint8_t)_buffer[off + 1]));
                }


            /**
            * return the size of the first available message
            * return 0 if none available. 
            **/
            int _msg_size()
                {
                if (_nb_msg == 0) return 0;
                const int l = _read_msg_buf_size(_ind_msg);
                if (l > 0) return l;                
                _ind_msg = 0;
                const int l2 = _read_msg_buf_size(0);
                // *************** HERE, l2 SHOULD ALWAYS BE NON-ZERO **************. 
                if (l2 == 0) { ABORT("SerialPacket: l2=0") }                    
                return l2;
                }



            inline void _mutex_wait()
                {
                while (!_mutex_start()) { threads.yield(); }
                }

      
            inline bool _mutex_start()
                {
                noInterrupts();
                if (_busy)
                    {
                    interrupts();
                    return false;
                    }
                _busy++;
                interrupts();
                return true;
                }


            inline void _mutex_stop()
                {
                noInterrupts();
                _busy--;
                interrupts();
                }


            Stream* _serial;                // serial port to use for communication

            uint8_t * _buffer;              // receive buffer
            int _buffer_len;                // size of the receive buffer

            volatile int _nb_msg;           // number of messages available in the receive buffer. 
            volatile int _ind_msg;          // position of the first available msg in the receive buffer
            volatile int _ind_free;         // position of the free space in the receive buffer. 

            volatile bool _escape_flag;     // set when the escape tag was received at the previous byte. 

            volatile uint8_t _id_send;      // contain the id of the last message send
            volatile uint8_t _id_ack;       // receive the id of the last ack received
            volatile uint8_t _id_recv;      // contain the id of the last message received

            CRC16             _crc_write;   // current crc for the output stream.
            CRC16             _crc_read;    // current crc for the input stream 

            volatile uint16_t _cur_crc_h;   // crc of the incomming msg (computed from the msg received)
            volatile uint16_t _cur_crc_s;   // crc of the incomming msg (embeded in the msg)

            volatile int      _cur_free;    // current position of the free index (ie position of the start of the msg).
            volatile uint16_t _cur_size;    // size of the payload in the incoming message (meaningless for an ack) 
            volatile uint16_t _cur_recv;    // number of bytes of the payload already received
            volatile uint8_t  _cur_id;      // id of the incoming message (>= 128 for an ack)            
            volatile uint8_t  _cur_state;   // current state

            volatile int _busy;             // true if an object method is ongoing.
            
        };



}

/** end of file */


