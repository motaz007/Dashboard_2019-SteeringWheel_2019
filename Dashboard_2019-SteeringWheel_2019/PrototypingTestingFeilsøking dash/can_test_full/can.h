/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * Uses FexCAN library to enable dashboard teensy to send and recieve CAN.
 * CAN at 500k
 */

#include <FlexCAN.h>

#ifndef CAN_H_
#define CAN_H_

/*-------------------------- CAN ID's --------------------------*/
#define swheelID 0x235
#define someOtherID 0x260

/*-------------------- Functions for CANbus --------------------*/
void canFilter();
void initCAN();
int writeCan(const CAN_message_t& msg);
int readCan(CAN_message_t& msg);
void printCanToSerial(const CAN_message_t& msg);







#endif
