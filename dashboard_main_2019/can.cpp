/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * Uses FexCAN library to enable dashboard teensy to send and recieve CAN.
 * CAN at 500k 
 */

#include "can.h"

int canID[] = {clutchID, sWheelID, motor1ID, motor2ID};                             //message ID's we want to recieve, error if not put here

void canFilter() {
  int numerOfIDs = sizeof(canID)/4;
  int filterInt[numerOfIDs+1];

  for (int j=0; j<numerOfIDs; j++) {                                                //devides 14 mailboxes on the different ID's
    filterInt[j] = (14.0/numerOfIDs)*j;
  }
  filterInt[numerOfIDs] = 14;                                                       //all messages will be sent through if not all 14 mailboxes are used, 
                                                                                    //therfore the last ID vill get the rest

  for (int i{0}; i<numerOfIDs; i++) {                                               //creates the 14 maiboxes for the ID's
    CAN_filter_t filter; 
    filter.id = canID[i];
    
    for (int filterNum=filterInt[i]; filterNum<filterInt[i+1]; filterNum++)
    {
    Can0.setMask(0x7FF<<18,filterNum);                                              //require exact match on filter.id, bitshift is necessary for 11-bit id's
    Can0.setFilter(filter,filterNum);
    }
  }
}

void initCAN()
{
  
  Serial.println("Initializing CANbus at 500k.");
  Can0.begin(CAN_BAUDRATE);  
  canFilter();                                                                      //set up filters so we only recieve usefull messages
}


void initCanMessage(CAN_message_t& msg, int length) 
{
  msg.ext = 0;
    
  msg.id = dashID;                                                                  //setting id
  msg.len = length;                                                                 //setting length
  
  for(int i=0; i<length; i++) { 
     msg.buf[i] = 0x00;                                                             //setting all values to zero
  }
}


int writeCan(const CAN_message_t& msg)
{ 
  return Can0.write(msg);
}

void readCan(CAN_message_t& rxMsg,CAN_message_t& sWheelMsg, CAN_message_t& motor1Msg, CAN_message_t& motor2Msg)
{
  
  
  while(Can0.available()) 
  {
   Can0.read(rxMsg);     //saves new can-message to rxMsg
      
   switch(rxMsg.id) {                                                                //sorts the message to the correct id to be used later
    case clutchID:
      clutchMsg = rxMsg;
      break;
    case sWheelID:
      sWheelMsg = rxMsg;
      break;
    case motor1ID:
      motor1Msg = rxMsg;
      break;
    case motor2ID:
      motor2Msg = rxMsg;
      break;
   }
   //printCanToSerial(rxMsg, true);
   //printCanToSerial(motor1Msg, true);
   //printCanToSerial(motor2Msg,true);
  }
  
  
}

void printCanToSerial(const CAN_message_t& msg, bool debug)
{
  if(debug){
    Serial.print(" ");                                                              //write message to serial on form: "id:length:data buf[1]: buf[2]..."
    Serial.print(msg.id, HEX);
    Serial.print(": ");
    Serial.print(msg.len);
    Serial.print(":");
    for (int i{0}; i<msg.len; ++i) {
      Serial.print(msg.buf[i], HEX);
      if (i != msg.len-1) {
        Serial.print("-");
      } 
    }
    Serial.print("  \n");
  }
}
