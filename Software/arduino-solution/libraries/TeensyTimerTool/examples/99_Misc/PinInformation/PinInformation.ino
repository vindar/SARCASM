#include "Arduino.h"

#include "PinInfo.h"
#include <algorithm>

void printPins(PinInfo* pins[], unsigned nrOfPins);

void setup()
{
    while (!Serial) {}

    // setup an array containing info for all digital pins
    PinInfo* pins[CORE_NUM_DIGITAL];
    for (unsigned i = 0; i < CORE_NUM_DIGITAL; i++)
    {
        pins[i] = new PinInfo(i);
    }

    // Print out info sorted by pin numbers
    Serial.println("-------------------------------");
    Serial.println("    Sorted by pin number");
    printPins(pins, CORE_NUM_DIGITAL);

    Serial.println("\n-------------------------------");
    Serial.println("     Sorted by PWM timer");
    std::sort(pins, pins + CORE_NUM_DIGITAL, [](PinInfo* a, PinInfo* b) {
        if (a->pwmTimerInfo.type < b->pwmTimerInfo.type) return false;
        if (a->pwmTimerInfo.type > b->pwmTimerInfo.type) return true;
        if (a->pwmTimerInfo.module < b->pwmTimerInfo.module) return true;
        return false;
    });
    printPins(pins, CORE_NUM_DIGITAL);

    Serial.println("\n-------------------------------");
    Serial.println("   Sorted by GPIO register:        ");
    std::sort(pins, pins + CORE_NUM_DIGITAL, [](PinInfo* a, PinInfo* b) {
        if (a->gpioInfo.gpioPortNr < b->gpioInfo.gpioPortNr) return true;
        if (a->gpioInfo.gpioPortNr > b->gpioInfo.gpioPortNr) return false;
        if (a->gpioInfo.gpioBitNr < b->gpioInfo.gpioBitNr) return true;
        return false;
    });
    printPins(pins, CORE_NUM_DIGITAL);
}

void loop() {}

// Helpers -------------------------------------------------------

void printPins(PinInfo* pins[], unsigned nrOfPins)
{
    Serial.println("Pin |  GPIO Reg  |  PWM timer");
    Serial.println("----|------------|-------------");
    for (unsigned i = 0; i < nrOfPins; i++)
    {
        Serial.printf("%02d  |  %-9s |  %-10s\n", pins[i]->pin, pins[i]->gpioInfo.name, pins[i]->pwmTimerInfo.name);
    }
}
