/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * Uses FexCAN library to enable dashboard teensy to send and recieve CAN.
 * CAN at 500k
 */

#include <FlexCAN.h>
#include "graphics.h"

#ifndef CAN_H_
#define CAN_H_

/*-------------------------- CAN ID's --------------------------*/
#define clutchID 0x120                                              //these are subject to change from year to year
#define sWheelID 0x230
#define motor1ID 0x250
#define motor2ID 0x260
#define dashID   0x270                                              //this is obviously the id for this system

#define CAN_BAUDRATE 500000                                         //CANbus in the FF car runs at 500k

static CAN_message_t txMsg,                                         //output
                     rxMsg,                                         //input (temp)
                     sWheelMsg,                                     //steering wheel
                     motor1Msg,                                     //motor 1, not sure nor important if left or right
                     motor2Msg,                                     //motor 2
                     clutchMsg;                                     //actuator board, used for gear. Only using one as these should be koordinated 
                                                                    //(if not it's probably better if the driver doesn't get that info...)

/*-------------------- Functions for CANbus --------------------*/
void canFilter();
void initCAN();
void initCanMessage(CAN_message_t& msg, int length);
int writeCan(const CAN_message_t& msg);
void readCan(CAN_message_t& msg);
void printCanToSerial(const CAN_message_t& msg, bool debug);

#endif
