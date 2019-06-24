/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * Uses FexCAN library to enable dashboard teensy to send and recieve CAN.
 * CAN at 500k 
 */

/*----------CAN-cable-----------------*/
// Yellow = 12V
// Green - CANH
// Red - CANL
// Black - GND

#include "can.h"

int canID[] = {swheelID, someOtherID};

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
  canFilter(); //set up filters
}


void initCanMessage(CAN_message_t& msg, int length) 
{
  msg.ext = 0;
    
  msg.id = dashID;                                                                 //setting id
  msg.len = length;                                                                //setting length
  
  for(int i=0; i<length; i++) { 
     msg.buf[i] = 0x00;                                                            //setting all values to zero
  }
}


int writeCan(const CAN_message_t& msg)
{ 
  return Can0.write(msg);
}

void readCan(CAN_message_t& msg)
{
  while(Can0.available()) 
  {
   Can0.read(msg);
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
