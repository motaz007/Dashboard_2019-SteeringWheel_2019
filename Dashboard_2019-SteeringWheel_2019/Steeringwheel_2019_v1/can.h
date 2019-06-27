/* Made SEM 2019
 * by Ludvik Rønning
 * Uses FexCAN library to enable dashboard teensy to send and recieve CAN.
 * CAN uses 500k speed
 */
 
#include <FlexCAN.h>

#ifndef CAN_H_
#define CAN_H_

/*-------------------------- CAN ID's --------------------------*/
#define dashID 0x270
#define swheelID 0x230
#define someOtherID 0x270


#define CAN_BAUDRATE 500000

/*-----------------------------------*/




/*-------------------- Functions for CANbus --------------------*/


void initCAN(void);
int writeCan(const CAN_message_t& msg);
void readCan(CAN_message_t& msg);
void printCanToSerial(CAN_message_t& msg, bool debug);
void initValuesSWheel(CAN_message_t& txMsg);

#endif
