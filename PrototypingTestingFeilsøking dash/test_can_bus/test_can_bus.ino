#include <FlexCAN.h>

#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif


static CAN_message_t msg;
static uint8_t hex[17] = "0123456789abcdef";

// -------------------------------------------------------------
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
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


// -------------------------------------------------------------
void setup(void)
{
  delay(1000);
  Serial.println(F("Hello Teensy 3.6 dual CAN Test."));

  Can0.begin(500000);  
  Can1.begin(500000);

  //if using enable pins on a transceiver they need to be set on
  // pinMode(2, OUTPUT);
  // pinMode(35, OUTPUT);

  // digitalWrite(2, HIGH);
  // digitalWrite(35, HIGH);

  msg.ext = 0;
  msg.id = 0x120;
  msg.len = 6;
  msg.buf[0] = 0x1B;
  msg.buf[1] = 0x70;
  msg.buf[2] = 0x30;
  msg.buf[3] = 0x29;
  msg.buf[4] = 0x11;
  msg.buf[5] = 0x3A;
  //msg.buf[6] = 0x10;
  //msg.buf[7] = 0x10;
}


// -------------------------------------------------------------
void loop(void)
{
  CAN_message_t inMsg;
  while (Can0.available()) 
  {
    Can0.read(inMsg);
    Serial.print("CAN bus 0: "); hexDump(8, inMsg.buf);
    Serial.print(inMsg.id);
  }
  Can0.write(msg);  
  
  
  if(Can0.available()) 
  {
    Can0.read(msg);
   Serial.print("CAN bus 0: "); hexDump(8, msg.buf);
  }
  delay(200);
}
