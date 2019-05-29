/* Made SEM 2019
 * by Ludvik Rønning
 * Uses FexCAN library to enable dashboard teensy to send and recieve CAN.
 * CAN uses 500k speed
 */
#include <FlexCAN.h>

#ifndef CAN_H_
#define CAN_H_

static uint8_t hex[17] = "0123456789abcdef";




/****** Functions for CANbus ******/

void hexDump(uint8_t dumpLen, uint8_t *bytePtr);
void initCAN(void);
int writeCan(const CAN_message_t& msg);
void readCan(CAN_message_t& msg);
void printCanToSerial(CAN_message_t& msg);

#endif
