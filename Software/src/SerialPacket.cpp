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

#include "SerialPacket.h"
#include <TeensyThreads.h>



// Packet layout
//
// For a 'real' packet (size =  msg_size+5 bytes if msg_size<255 or msg_size+ 7 if 255 <= msg_size < 65535). 
//
// - [1 byte] SERIAL_PACKET_TAGSTART
// - [1 byte] ID. The id is < 128 (ie high bit is 0), increase by one at each new packet. 
// - [1 or 3 bytes] length of payload in bytes: if first byte < 255, this is the length, otherwise, the following two bytes give the size (low byte followed by high byte). 
// - [at least 1 byte] the payload of the packet. 
// - [2 bytes] checksum of the packet CRC16 CCITT (low byte followed by high byte). 
//
// For an acknowledgment (size = 4 bytes). 
//
// - [1 byte] SERIAL_PACKET_TAGSTART
// - [1 byte] ID. the high bit is set at 1, bit 0-7 give the id of the packet beiing acknowledged 
// - [2 bytes] checksum of the packet CRC16 CCITT (low byte followed by high byte). 



namespace SerialPacket
    {



        SLOWFUN void SerialPacket::begin(Stream* serial, uint8_t * buffer_recv, int len)
            {
            _mutex_wait();
            _serial = serial;

            _buffer = buffer_recv;
            _buffer_len = len;

            _nb_msg = 0;
            _ind_msg = 0;
            _ind_free = 0;

            _escape_flag = false;

            _id_send = 0;
            _id_ack = 0;
            _id_recv = 0;

            _cur_state = SERIAL_PACKET_STATE_IDLE;
            _mutex_stop();
            }



        SLOWFUN void SerialPacket::send(const char* buffer, uint16_t len, bool confirm, int retry_ms)
            {
            if ((len == 0) || (buffer == nullptr)) return; // input parameters validation                
            _mutex_wait();
            _id_send = (_id_send + 1) & 127; // id of the message to send 
            while (1)
                {    
                // send the tag
                _writeFrameStart();

                // send the id
                _write(_id_send);
                
                // send the size               
                if (len < 254)
                    { // short message (payload <= 254 bytes). 
                    _write((uint8_t)len);
                    }
                else
                    { // long message (payload in [255, 65535] bytes)
                    _write(255);
                    _write((uint8_t)(len & 255));
                    _write((uint8_t)(len >>  8));
                    }

                // send the payload
                _write((const uint8_t*)buffer, len);

                // send the crc
                uint16_t val = _get_write_crc();
                _write(val & 255);
                _write(val >> 8);


                // make sure everything is sent right away.
                _serial->flush();              

                // wait for ack
                if (!confirm)
                    {
                    _mutex_stop();
                    return;
                    }
                const uint32_t t = millis();
                _poll();

                
                while ((_id_ack != _id_send)&&((millis() - t) < (uint32_t)retry_ms))
                    {
                    threads.yield();
                    _poll();
                    }

                if (_id_ack == _id_send)
                    {// ok, ack received !
                    _mutex_stop();
                    return;
                    }

                // ... try sending the message again.
                // *************** ERROR : resend message ************************
                SERIAL_PACKET_DEBUG("send() SENDING MESSAGE AGAIN.  id: " << _id_send << " size: " <<  len)
                }
            }


     
        SLOWFUN int SerialPacket::wait(int timeout_ms)
            {
            const uint32_t t = millis();
            _mutex_wait();
            while (1)
                {
                _poll();
                const int n = _nb_msg;
                if (n > 0)
                    {
                    _mutex_stop();
                    return _nb_msg;
                    }
                if ((timeout_ms >= 0) && (((uint32_t)millis()) - t >= ((uint32_t)timeout_ms))) 
                    {
                    _mutex_stop();
                    return 0;
                    }
                threads.yield();
                }
            }


        SLOWFUN int SerialPacket::size()
            {
            _mutex_wait();
            const int l = _msg_size();
            _mutex_stop();
            return l;
            }


        SLOWFUN const char* SerialPacket::recv_buffer()
            {
            _mutex_wait();
            int l = _msg_size();
            _mutex_stop();
            if (l == 0) return nullptr;
            return (((const char*)_buffer) + (_ind_msg + 2));
            }


        SLOWFUN void SerialPacket::next()
            {
            _mutex_wait();
            const int l = _msg_size();
            if (l == 0)
                {
                _mutex_stop();
                return;
                }
            --_nb_msg; // decrease then number of messages
            _ind_msg += l + 2; // go to the beginning of the next msg
            _msg_size(); // correct the offset if needed; 
            _mutex_stop();
            }   




        SLOWFUN void SerialPacket::_poll()
            {
            while (1)
                {                
                const int c = _read();
                if (c == -1) return; // nothing to do.           
                if (c == -2)
                    { // new frame
                    if (_cur_state != SERIAL_PACKET_STATE_IDLE)
                        {
                        // ************** ERROR : frame interrupted by start tag.  **************     
                        SERIAL_PACKET_DEBUG("poll() TAG INTERRUPTION")
                        }
                    _cur_state = SERIAL_PACKET_STATE_ID; // 1 = start of frame. 
                    }
                else
                    { // got a char.
                    switch (_cur_state)
                        {
                        case SERIAL_PACKET_STATE_IDLE: 
                            // ************** ERROR : garbage received **************                            
                            SERIAL_PACKET_DEBUG("poll() garbage received:" << (int)c)
                            break; // just ignore char

                        case SERIAL_PACKET_STATE_ID:
                            _cur_id = c; // save packet id      
                            if (c < 128)
                                { // this is a message
                                _cur_state = SERIAL_PACKET_STATE_SIZE1;
                                }
                            else
                                { // this is an ack
                                _cur_crc_h = _get_read_crc(); // save the crc of the message
                                _cur_state = SERIAL_PACKET_STATE_CRC1;
                                }                            
                            break;

                        case SERIAL_PACKET_STATE_SIZE1:
                            if (c == 255)
                                { // large packet
                                _cur_state = SERIAL_PACKET_STATE_SIZE2;
                                }
                            else
                                { // small packet
                                _cur_size = (uint16_t)c;  
                                if (_cur_size == 0)
                                    {
                                    // ************** ERROR : incorrect message size 0 **************
                                    SERIAL_PACKET_DEBUG("poll() incorrect message size (=0)")
                                    _cur_state = SERIAL_PACKET_STATE_IDLE;
                                    }
                                else
                                    {
                                    _cur_state = SERIAL_PACKET_STATE_PAYLOAD_FIRST;
                                    }                                
                                }
                            break;

                        case SERIAL_PACKET_STATE_SIZE2:
                            _cur_size = (uint16_t)c; // low byte commes first
                            _cur_state = SERIAL_PACKET_STATE_SIZE3;
                            break;

                        case SERIAL_PACKET_STATE_SIZE3:
                            _cur_size += (uint16_t)(256 * c); // followed by high byte                            
                            if (_cur_size == 0)
                                {
                                // ************** ERROR : incorrect message size 0 **************
                                SERIAL_PACKET_DEBUG("poll() incorrect message size bis (=0)")
                                _cur_state = SERIAL_PACKET_STATE_IDLE;
                                }
                            else
                                {
                                _cur_state = SERIAL_PACKET_STATE_PAYLOAD_FIRST;
                                }                                
                            break;

                        case SERIAL_PACKET_STATE_PAYLOAD_FIRST:
                            // first byte of the payload, we first find some room in the buffer to store the message. 
                            _cur_recv = 0; // no byte previously received
                            if (_nb_msg == 0)
                                { // reset positions whevever queue is empty 
                                _ind_free = 0;
                                _ind_msg = 0;
                                }
                            if (_ind_free < _ind_msg)
                                {
                                if (_ind_msg - _ind_free < _cur_size + 4)
                                    { 
                                    // ************** ERROR : buffer overflow **************
                                    SERIAL_PACKET_DEBUG("poll() buffer overflow A " << _ind_msg - _ind_free << " / " << _cur_size)
                                    _cur_state = SERIAL_PACKET_STATE_IDLE;
                                    break;
                                    }
                                _cur_free = _ind_free; // ok, we can put the message starting at index _ind_free
                                }
                            else
                                {                                 
                                if (_buffer_len - _ind_free < _cur_size + 4)
                                    { // not enough room. Try to put message at the beginning. 
                                    if (_ind_msg < _cur_size + 4)
                                        { 
                                        // ************** ERROR : buffer overflow **************
                                        SERIAL_PACKET_DEBUG("poll() buffer overflow B" <<  _ind_msg  << " / " << _cur_size)
                                        _cur_state = SERIAL_PACKET_STATE_IDLE;
                                        break;
                                        }
                                    _write_msg_buf_size(_ind_free, 0); // tell that the msg will be at the start of buffer.
                                    _cur_free = 0; 
                                    }
                                else
                                    {
                                    _cur_free = _ind_free; // ok, we can put the message starting at index _ind_free
                                    }                                
                                }
                            _write_msg_buf_size(_cur_free, _cur_size); // write the message size. 
                            // No break here: we directly process the first byte. 
                            _cur_state = SERIAL_PACKET_STATE_PAYLOAD_NEXT;

                        case SERIAL_PACKET_STATE_PAYLOAD_NEXT:
                            
                            _buffer[_cur_free + _cur_recv + 2] = (uint8_t)c; // save the byte
                            _cur_recv++; // increase count

                            if (_cur_recv == _cur_size)
                                { // done with payload. 
                                _cur_crc_h = _get_read_crc();
                                _cur_state = SERIAL_PACKET_STATE_CRC1;
                                }
                            break;

                        case SERIAL_PACKET_STATE_CRC1:
                            _cur_crc_s = (uint16_t)c;
                            _cur_state = SERIAL_PACKET_STATE_CRC2;
                            break;

                        case SERIAL_PACKET_STATE_CRC2:                           
                            _cur_crc_s += (uint16_t)(256*c);
                            _cur_state = SERIAL_PACKET_STATE_IDLE;
                            if (_cur_crc_s != _cur_crc_h)
                                {
                                // ************** ERROR : wrong checksum **************
                                SERIAL_PACKET_DEBUG("poll() wrong checksum.  expected=" << _cur_crc_s  << " computed=" << _cur_crc_h)
                                break;
                                }

                            // ok, message received correctly. 
                            if (_cur_id >= 128)
                                { // received an ack
                                _id_ack = (_cur_id & 127);
                                }
                            else
                                { // received a message                                
                                _send_ack(_cur_id); // send back ack
                                if (_id_recv != _cur_id)
                                    { // it is a new message so we make it available. 
                                    _id_recv = _cur_id;
                                    _nb_msg++;
                                    _ind_free = _cur_free + _cur_size + 2;
                                    }
                                else
                                    {
                                    SERIAL_PACKET_DEBUG("poll() message already received, id ="<< _id_recv);
                                    }
                                }                            
                            break;
                        }
                    }
                }            
            }


        SLOWFUN void SerialPacket::_send_ack(uint8_t id)
            {
            // send the tag
            _writeFrameStart();

            // send the id with high bit set
            _write(id | 128);            

            // get the ack. crc
            uint16_t val = _get_write_crc();

            // send the crc
            _write(val & 255); // lowest byte first
            _write(val >> 8);  // then highest one

            // make sure everything is sent right away.
            _serial->flush();

            }


        SLOWFUN int SerialPacket::_read()
            {
            while (1)
                {
                int c = _serial->read();
                if (c < 0) return -1;
                if (c == SERIAL_PACKET_TAGSTART)
                    {
                    _escape_flag = false;
                    const uint8_t b = SERIAL_PACKET_TAGSTART;
                    _crc_read.ccitt(&b, 1);
                    return -2;
                    }
                if (_escape_flag)
                    {
                    _escape_flag = false;
                    if (c == SERIAL_PACKET_ESCAPE)
                        {
                        const uint8_t b = (uint8_t)SERIAL_PACKET_ESCAPE;
                        _crc_read.ccitt_upd(&b, 1);
                        return SERIAL_PACKET_ESCAPE;
                        }
                    else
                        {
                        const uint8_t b = (uint8_t)SERIAL_PACKET_TAGSTART;
                        _crc_read.ccitt_upd(&b, 1);
                        return SERIAL_PACKET_TAGSTART;
                        }
                    }
                if (c != SERIAL_PACKET_ESCAPE)
                    {
                    const uint8_t b = (uint8_t)c;
                    _crc_read.ccitt_upd(&b, 1);
                    return c;
                    }
                _escape_flag = true;
                }
            }


        SLOWFUN void SerialPacket::_write(uint8_t c)
            {
            _crc_write.ccitt_upd(&c, 1);
            switch (c)
                {
                case SERIAL_PACKET_TAGSTART:
                    _serial->write((uint8_t)SERIAL_PACKET_ESCAPE);
                    _serial->write((uint8_t)0);
                    break;

                case SERIAL_PACKET_ESCAPE:
                    _serial->write((uint8_t)SERIAL_PACKET_ESCAPE);
                    _serial->write((uint8_t)SERIAL_PACKET_ESCAPE);
                    break;

                default:
                    _serial->write(c);
                    break;
                }
            }


        SLOWFUN void SerialPacket::_write(const uint8_t* buffer, int len)
            {
            _crc_write.ccitt_upd(buffer, len);
            int k0 = 0;
            int k1 = 0;
            while (k1 < len)
                {
                const uint8_t c = buffer[k1];
                if ((c == SERIAL_PACKET_TAGSTART) || (c == SERIAL_PACKET_ESCAPE))
                    {
                    if (k1 > k0) _serial->write((const char*)buffer + k0, k1 - k0);
                    _serial->write((uint8_t)SERIAL_PACKET_ESCAPE);
                    if (c == SERIAL_PACKET_ESCAPE) _serial->write((uint8_t)SERIAL_PACKET_ESCAPE); else _serial->write((uint8_t)0);
                    k1++;
                    k0 = k1;
                    }
                else
                    {
                    k1++;
                    }
                }
            // write the remaining bytes in the buffer. 
            if (k1 > k0) _serial->write((const char*)buffer + k0, k1 - k0);
            }





}


/** end of file */


