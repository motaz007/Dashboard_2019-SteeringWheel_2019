#include "arduino.h"

#ifndef HELPERS_H_
#define HELPERS_H_

#define OPTIMAL_CURRENT_VAL 60
#define OPTIMAL_BRAKE_VAL   60 

#define CC_COUNTER_MAX 6

#define SERIAL_BAUDRATE 9600



int readThrottle(int PIN_THROTTLE, volatile uint8_t buttons[], bool ccActive, bool optimalCurrent);
int readRegen(int PIN_REGEN, volatile uint8_t *buttons, bool ccActive, bool optimalBrakeActive);
void initSerial();
void clockSpeed2Mhz(bool debug);
void clockSpeed120Mhz(bool debug);

#endif
