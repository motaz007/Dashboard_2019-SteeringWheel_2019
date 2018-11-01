
#include <FlexCAN.h>

#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif


static CAN_message_t msg;
static CAN_message_t id;
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
  Can0.begin(500000);

  delay(1000);
  Serial.println(F("Hello Teensy 3.6 dual CAN Test."));
}


// -------------------------------------------------------------
void loop(void)
{
  if(Can0.available()) 
  {
    Can0.read(msg);
    Can0.read(id);
  
   Serial.print("CAN bus 0: "); hexDump(8, id.buf);
  }

}
