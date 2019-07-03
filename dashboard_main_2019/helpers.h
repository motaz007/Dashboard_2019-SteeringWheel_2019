/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * 
 */

#include "arduino.h"

#ifndef HELPERS_H_
#define HELPERS_H_

#define SERIAL_BAUDRATE 9600
#define OPTIMALCOUNTER_MAX 10



void initSerial();
void clockSpeedLow(bool debug);
void clockSpeedHigh(bool debug);

#endif
