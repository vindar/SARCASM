#include "SystemController.h"

// --> Uncomment if you have a control voltage on the pot pin  <--
// #include "ResponsiveAnalogRead.h"
// constexpr unsigned potPin = A0;
// ResponsiveAnalogRead pot(potPin, false);

SystemController sysControl;

void setup()
{
    sysControl.begin();

    sysControl.setExposureDelay(250); // set exposure delay (time between two exposures) to 250 µs
    sysControl.shoot();               // do a manual exposure
    delay(10);
    sysControl.setExposureDelay(500); // same with 500µs delay between exposures
    sysControl.shoot();

    sysControl.continuosMode(true);   // start continuously shooting
    delay(1000);
    sysControl.continuosMode(false);  // stop after one second

    delay(500);
    sysControl.continuosMode(true);   // start again
}

void loop()
{
    // --> Uncomment if you have a control voltage on the pot pin  <--
    // pot.update();
    // if (pot.hasChanged())
    // {
    //     unsigned expDelay = map(pot.getValue(), 0, 1023, 100, 500); // 0-3.3V analog value, maps to 100-500
    //     controller.setExposureDelay(expDelay);
    //     Serial.printf("Exposure Delay: %u µs\n", expDelay);
    // }
}
