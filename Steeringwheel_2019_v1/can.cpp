/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * Uses FlexCAN library to enable dashboard teensy to send and recieve CAN.
 * CAN at 500k
 */
#include "can.h"

uint8_t txAlt = 1;
uint8_t rxAlt = 1;

void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working>>4 ] );
    Serial.write( hex[ working&15 ] );
  }
  Serial.write('\r');
  Serial.write('\n');
}

void initCAN(void)
{
  Serial.begin(9600);
  delay(1000);
  Serial.println(F("Initializing CANbus at 500k."));

  Can0.begin(500000);  
  //Can1.begin(500000); //We are only using one bus
  
  Serial.print("check");
  delay(1000);
  
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
   printCanToSerial(msg);
   
  }
  
}

void printCanToSerial(CAN_message_t& msg)
{
   Serial.print("CAN bus 1: "); hexDump(8, msg.buf);
   Serial.print(msg.id,HEX);
}
