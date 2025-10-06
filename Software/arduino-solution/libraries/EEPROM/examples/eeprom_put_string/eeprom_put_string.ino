/***
    eeprom_put_string example.

    This shows how to use the EEPROM.put() method to write Arduino Strings to the EEPROM
    Also, this sketch will pre-set the EEPROM data for the
    example sketch eeprom_get_string.

    Note, unlike the single byte version EEPROM.write(),
    the put method will use update semantics. As in a String
    will only be written to the EEPROM if the data is actually
    different.
    luni64, April 2022
    Released under MIT licence.
***/

#include <EEPROM.h>

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for boards with integrated USB only
  }

  String s1 = "Hello EEPROM";   // Variable to write to EEPROM.
  unsigned int eeAddress = 0;   // Location we want the data to be put.

  EEPROM.put(eeAddress, s1);    // One simple call, with the address first and the object second.
  eeAddress += s1.length() + 1; // Update address to the next byte after String s1.
                                // Don't forget to add 1 to take the additionally written String terminator (\0) into account.
  Serial.print("Written \"");
  Serial.print(s1);
  Serial.println("\" to EEPROM");

  for (int i = 0; i < 5; i++)
  {
    String s = "This is string #" + String(i);

    if (s.length() + 1 <= EEPROM.length() - eeAddress) // Check for free EEPROM space.
    {
      EEPROM.put(eeAddress, s);     //Write to EEPROM.
      eeAddress += s.length() + 1;  //Update address. Take the additionally written String terminator (\0) into account.

      Serial.print("Written \"");
      Serial.print(s);
      Serial.println("\" to EEPROM");
    }
    else
    {
      Serial.println("No free space in EEPROM");
      break;
    }
  }

  Serial.println("\nView the example sketch eeprom_get_string to see how you can retrieve the values!");
}

void loop() { /* Empty loop */
}
