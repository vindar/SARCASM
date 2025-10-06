
/****************************************************
  AMS 5600 class for Arduino platform
  Author: Tom Denton
  Date: 15 Dec 2014
  File: AMS_5600.h 
  Version 1.00
  www.ams.com
   
  Description:  This class has been designed to
  access the AMS 5600 “potuino” shield.
***************************************************/

// updated jan 2022 by isc - read two bytes together

// datasheet: https://ams.com/documents/20143/36005/AS5600_DS000365_5-00.pdf

#ifndef AMS_5600_h
#define AMS_5600_h

#include <Arduino.h>

class AMS_5600
{
public:

  AMS_5600(void);
  int getAddress();

  word setMaxAngle(word newMaxAngle = -1);
  word getMaxAngle();

  word setStartPosition(word startAngle = -1);
  word getStartPosition();

  word setEndPosition(word endAngle = -1);
  word getEndPosition();

  word getRawAngle();
  word getScaledAngle();

  int detectMagnet();
  int getMagnetStrength();
  int getAgc();
  word getMagnitude();
  word getConf();
  void setConf(word _conf);

  int getBurnCount();
  int burnAngle();
  int burnMaxAngleAndConfig();
  void setOutPut(uint8_t mode);

private:

  // i2c address
  static const uint8_t _ams5600_Address = 0x36;
  
  // single byte registers
  static const uint8_t _addr_status = 0x0b; // magnet status
  static const uint8_t _addr_agc    = 0x1a; // automatic gain control
  static const uint8_t _addr_burn   = 0xff; // permanent burning of configs (zpos, mpos, mang, conf)
  static const uint8_t _addr_zmco   = 0x00; // number of times zpos/mpos has been permanently burned
                                            // zpos/mpos can be permanently burned 3x
                                            // mang/conf can be burned only once
  
  // double byte registers, specify starting address (lower addr, but higher byte data)
  // addr   = upper byte of data (MSB), only bits 0:3 are used
  // addr+1 = lower byte of data (LSB)
  static const uint8_t _addr_zpos      = 0x01; // zero position (start)
                                               // 0x02 - lower byte
  static const uint8_t _addr_mpos      = 0x03; // maximum position (stop)
                                               // 0x04 - lower byte
  static const uint8_t _addr_mang      = 0x05; // maximum angle
                                               // 0x06 - lower byte
  static const uint8_t _addr_conf      = 0x07; // configuration
                                               // 0x08 - lower byte
  static const uint8_t _addr_raw_angle = 0x0c; // raw angle
                                               // 0x0d - lower byte
  static const uint8_t _addr_angle     = 0x0e; // mapped angle
                                               // 0x0f - lower byte
  static const uint8_t _addr_magnitude = 0x1b; // magnitude of internal CORDIC
                                               // 0x1c - lower byte

  int readOneByte(int in_adr);
  word readTwoBytesSeparately(int addr_in);
  word readTwoBytesTogether(int addr_in);
  void writeOneByte(int adr_in, int dat_in);

};
#endif