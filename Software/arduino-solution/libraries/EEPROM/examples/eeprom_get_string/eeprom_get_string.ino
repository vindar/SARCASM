#include "Arduino.h"
/***
    eeprom_get example.

    This shows how to use the EEPROM.get() method to read Arduino Strings from the EEPROM.

    To pre-set the EEPROM data, run the example sketch eeprom_put_string.
    This sketch will run without it, however, the values shown
    will be shown from what ever is already on the EEPROM.

    This may cause the serial object to print out a large string
    of garbage if there is no null character inside one of the strings
    loaded.

    Written by luni64 April 2022
    Released under MIT licence.
***/

#include <EEPROM.h>

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for boards with built in USB only
  }
  Serial.println("Read 6 Strings from EEPROM: "); //To pre-set the EEPROM data, run the example sketch eeprom_put_string.

  String s;                     // Variable to store data read from EEPROM.
  unsigned int eeAddress = 0;   // EEPROM address to start reading from

  EEPROM.get(eeAddress, s);     // Get the float data from the EEPROM at position 'eeAddress'
  eeAddress += s.length() + 1;  // Advance read address to next String, add 1 to account for the leading \0
  Serial.println(s);            // This may print print out a large string of garbage if there is no null character inside the string

  for (int i = 0; i < 5; i++)
  {
    EEPROM.get(eeAddress, s);
    eeAddress += s.length() + 1;
    Serial.println(s);
  }
}

void loop() { /* Empty loop */
}
