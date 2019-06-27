/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * 
 */

#include "helpers.h"

int SERIAL_BAUDRATE = 9600;

void initSerial() {
    // to communicate with the computer
    Serial.begin(SERIAL_BAUDRATE);
}
