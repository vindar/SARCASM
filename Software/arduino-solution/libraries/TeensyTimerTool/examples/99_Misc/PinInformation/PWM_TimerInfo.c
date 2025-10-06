#include "core_pins.h"

/*****
 * Unlike Teensy 4.x there is no information array in the core for the T3.x series
 * We thus build the corresponding array from copies of the pin definitions in core_pins.h
 *****/


#if defined(ARDUINO_TEENSYLC)

const int PwmTimerModule[CORE_NUM_DIGITAL] = {
    [0 ... CORE_NUM_DIGITAL - 1] = -1,
    [CORE_TPM0_CH0_PIN] = 0,
    [CORE_TPM0_CH1_PIN] = 0,
    [CORE_TPM0_CH2_PIN] = 0,
    [CORE_TPM0_CH3_PIN] = 0,
    [CORE_TPM0_CH4_PIN] = 0,
    [CORE_TPM0_CH5_PIN] = 0,
    [CORE_TPM1_CH0_PIN] = 1,
    [CORE_TPM1_CH1_PIN] = 1,
    [CORE_TPM2_CH0_PIN] = 2,
    [CORE_TPM2_CH1_PIN] = 2,
};

const int PwmTimerType[CORE_NUM_DIGITAL] = {
    [0 ... CORE_NUM_DIGITAL - 1] = 0,
    [CORE_TPM0_CH0_PIN] = 2,
    [CORE_TPM0_CH1_PIN] = 2,
    [CORE_TPM0_CH2_PIN] = 2,
    [CORE_TPM0_CH3_PIN] = 2,
    [CORE_TPM0_CH4_PIN] = 2,
    [CORE_TPM0_CH5_PIN] = 2,
    [CORE_TPM1_CH0_PIN] = 2,
    [CORE_TPM1_CH1_PIN] = 2,
    [CORE_TPM2_CH0_PIN] = 2,
    [CORE_TPM2_CH1_PIN] = 2,
};

//===========================================================================================
#elif defined(ARDUINO_TEENSY30)

const int PwmTimerModule[CORE_NUM_DIGITAL] = {
    [0 ... CORE_NUM_DIGITAL - 1] = -1,
    [CORE_FTM0_CH0_PIN] = 0,
    [CORE_FTM0_CH1_PIN] = 0,
    [CORE_FTM0_CH2_PIN] = 0,
    [CORE_FTM0_CH3_PIN] = 0,
    [CORE_FTM0_CH4_PIN] = 0,
    [CORE_FTM0_CH5_PIN] = 0,
    [CORE_FTM0_CH6_PIN] = 0,
    [CORE_FTM0_CH7_PIN] = 0,
    [CORE_FTM1_CH0_PIN] = 1,
    [CORE_FTM1_CH1_PIN] = 1,
};

const int PwmTimerType[CORE_NUM_DIGITAL] = {
    [0 ... CORE_NUM_DIGITAL - 1] = 0,
    [CORE_FTM0_CH0_PIN] = 1,
    [CORE_FTM0_CH1_PIN] = 1,
    [CORE_FTM0_CH2_PIN] = 1,
    [CORE_FTM0_CH3_PIN] = 1,
    [CORE_FTM0_CH4_PIN] = 1,
    [CORE_FTM0_CH5_PIN] = 1,
    [CORE_FTM0_CH6_PIN] = 1,
    [CORE_FTM0_CH7_PIN] = 1,
    [CORE_FTM1_CH0_PIN] = 1,
    [CORE_FTM1_CH1_PIN] = 1,
};

//===========================================================================================
#elif defined(__MK20DX256__)
const int PwmTimerModule[CORE_NUM_DIGITAL] = {
    [0 ... CORE_NUM_DIGITAL - 1] = -1,
    [CORE_FTM0_CH0_PIN] = 0,
    [CORE_FTM0_CH1_PIN] = 0,
    [CORE_FTM0_CH0_PIN] = 0,
    [CORE_FTM0_CH1_PIN] = 0,
    [CORE_FTM0_CH2_PIN] = 0,
    [CORE_FTM0_CH3_PIN] = 0,
    [CORE_FTM0_CH4_PIN] = 0,
    [CORE_FTM0_CH5_PIN] = 0,
    [CORE_FTM0_CH6_PIN] = 0,
    [CORE_FTM0_CH7_PIN] = 0,
    [CORE_FTM1_CH0_PIN] = 1,
    [CORE_FTM1_CH1_PIN] = 1,
    [CORE_FTM2_CH0_PIN] = 2,
    [CORE_FTM2_CH1_PIN] = 2,
};

//===========================================================================================
const int PwmTimerType[CORE_NUM_DIGITAL] = {
    [0 ... CORE_NUM_DIGITAL - 1] = 0,
    [CORE_FTM0_CH0_PIN] = 1,
    [CORE_FTM0_CH1_PIN] = 1,
    [CORE_FTM0_CH0_PIN] = 1,
    [CORE_FTM0_CH1_PIN] = 1,
    [CORE_FTM0_CH2_PIN] = 1,
    [CORE_FTM0_CH3_PIN] = 1,
    [CORE_FTM0_CH4_PIN] = 1,
    [CORE_FTM0_CH5_PIN] = 1,
    [CORE_FTM0_CH6_PIN] = 1,
    [CORE_FTM0_CH7_PIN] = 1,
    [CORE_FTM1_CH0_PIN] = 1,
    [CORE_FTM1_CH1_PIN] = 1,
    [CORE_FTM2_CH0_PIN] = 1,
    [CORE_FTM2_CH1_PIN] = 1,
};

//===========================================================================================
#elif defined(ARDUINO_TEENSY35)
const int PwmTimerModule[CORE_NUM_DIGITAL] = {
    [0 ... CORE_NUM_DIGITAL - 1] = -1,
    [CORE_FTM0_CH0_PIN] = 0,
    [CORE_FTM0_CH1_PIN] = 0,
    [CORE_FTM0_CH2_PIN] = 0,
    [CORE_FTM0_CH3_PIN] = 0,
    [CORE_FTM0_CH4_PIN] = 0,
    [CORE_FTM0_CH5_PIN] = 0,
    [CORE_FTM0_CH6_PIN] = 0,
    [CORE_FTM0_CH7_PIN] = 0,
    [CORE_FTM1_CH0_PIN] = 1,
    [CORE_FTM1_CH1_PIN] = 1,
    [CORE_FTM2_CH0_PIN] = 2,
    [CORE_FTM2_CH1_PIN] = 2,
    [CORE_FTM3_CH0_PIN] = 3,
    [CORE_FTM3_CH1_PIN] = 3,
    [CORE_FTM3_CH2_PIN] = 3,
    [CORE_FTM3_CH3_PIN] = 3,
    [CORE_FTM3_CH4_PIN] = 3,
    [CORE_FTM3_CH5_PIN] = 3,
    [CORE_FTM3_CH6_PIN] = 3,
    [CORE_FTM3_CH7_PIN] = 3,
};

const int PwmTimerType[CORE_NUM_DIGITAL] = {
    [0 ... CORE_NUM_DIGITAL - 1] = 0,
    [CORE_FTM0_CH0_PIN] = 1,
    [CORE_FTM0_CH1_PIN] = 1,
    [CORE_FTM0_CH2_PIN] = 1,
    [CORE_FTM0_CH3_PIN] = 1,
    [CORE_FTM0_CH4_PIN] = 1,
    [CORE_FTM0_CH5_PIN] = 1,
    [CORE_FTM0_CH6_PIN] = 1,
    [CORE_FTM0_CH7_PIN] = 1,
    [CORE_FTM1_CH0_PIN] = 1,
    [CORE_FTM1_CH1_PIN] = 1,
    [CORE_FTM2_CH0_PIN] = 1,
    [CORE_FTM2_CH1_PIN] = 1,
    [CORE_FTM3_CH0_PIN] = 1,
    [CORE_FTM3_CH1_PIN] = 1,
    [CORE_FTM3_CH2_PIN] = 1,
    [CORE_FTM3_CH3_PIN] = 1,
    [CORE_FTM3_CH4_PIN] = 1,
    [CORE_FTM3_CH5_PIN] = 1,
    [CORE_FTM3_CH6_PIN] = 1,
    [CORE_FTM3_CH7_PIN] = 1,
};

//===========================================================================================
#elif defined(ARDUINO_TEENSY36)
const int PwmTimerModule[CORE_NUM_DIGITAL] = {
    [0 ... CORE_NUM_DIGITAL - 1] = -1,
    [CORE_FTM0_CH0_PIN] = 0,
    [CORE_FTM0_CH1_PIN] = 0,
    [CORE_FTM0_CH2_PIN] = 0,
    [CORE_FTM0_CH3_PIN] = 0,
    [CORE_FTM0_CH4_PIN] = 0,
    [CORE_FTM0_CH5_PIN] = 0,
    [CORE_FTM0_CH6_PIN] = 0,
    [CORE_FTM0_CH7_PIN] = 0,
    [CORE_FTM1_CH0_PIN] = 1,
    [CORE_FTM1_CH1_PIN] = 1,
    [CORE_FTM2_CH0_PIN] = 2,
    [CORE_FTM2_CH1_PIN] = 2,
    [CORE_FTM3_CH0_PIN] = 3,
    [CORE_FTM3_CH1_PIN] = 3,
    [CORE_FTM3_CH2_PIN] = 3,
    [CORE_FTM3_CH3_PIN] = 3,
    [CORE_FTM3_CH4_PIN] = 3,
    [CORE_FTM3_CH5_PIN] = 3,
    [CORE_FTM3_CH6_PIN] = 3,
    [CORE_FTM3_CH7_PIN] = 3,
    [CORE_TPM1_CH0_PIN] = 1,
    [CORE_TPM1_CH1_PIN] = 1,
};

const int PwmTimerType[CORE_NUM_DIGITAL] = {
    [0 ... CORE_NUM_DIGITAL - 1] = 0,
    [CORE_FTM0_CH0_PIN] = 1,
    [CORE_FTM0_CH1_PIN] = 1,
    [CORE_FTM0_CH2_PIN] = 1,
    [CORE_FTM0_CH3_PIN] = 1,
    [CORE_FTM0_CH4_PIN] = 1,
    [CORE_FTM0_CH5_PIN] = 1,
    [CORE_FTM0_CH6_PIN] = 1,
    [CORE_FTM0_CH7_PIN] = 1,
    [CORE_FTM1_CH0_PIN] = 1,
    [CORE_FTM1_CH1_PIN] = 1,
    [CORE_FTM2_CH0_PIN] = 1,
    [CORE_FTM2_CH1_PIN] = 1,
    [CORE_FTM3_CH0_PIN] = 1,
    [CORE_FTM3_CH1_PIN] = 1,
    [CORE_FTM3_CH2_PIN] = 1,
    [CORE_FTM3_CH3_PIN] = 1,
    [CORE_FTM3_CH4_PIN] = 1,
    [CORE_FTM3_CH5_PIN] = 1,
    [CORE_FTM3_CH6_PIN] = 1,
    [CORE_FTM3_CH7_PIN] = 1,
    [CORE_TPM1_CH0_PIN] = 2,
    [CORE_TPM1_CH1_PIN] = 2,
};

#endif
