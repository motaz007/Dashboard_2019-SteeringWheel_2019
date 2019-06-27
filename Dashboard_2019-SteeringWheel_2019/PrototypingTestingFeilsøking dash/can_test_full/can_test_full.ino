/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * 
 */

#include "can.h"
#include "helpers.h"

/*-------------------- COMM PARAMETERS --------------------*/

// BUTTONS
#define PIN_BRAKE_ENABLED    26
#define PIN_LIGHT_ENABLE     28
#define PIN_MAIN_SWITCH      37  // is this needed? everything turns off with the emergency switch anyways...
#define PIN_HAZARD_LIGHT     16
#define PIN_GEAR_2           29  // up
#define PIN_GEAR_1           25  // down
#define PIN_RESET            30
#define PIN_GEAR_AUTO_MANUAL 12

// precalculated bitmask (1<<bp) (bp = bit position) is supposed to be a little bit faster
#define GEAR_2_bp 1
#define GEAR_2_bm (1<<GEAR_2_bp)
#define GEAR_1_bp 0
#define GEAR_1_bm (1<<GEAR_1_bp)
#define GEAR_AUTO_MANUAL_bp 2
#define GEAR_AUTO_MANUAL_bm (1<<GEAR_AUTO_MANUAL_bp)

#define blueLed 22
#define greenLed 23
#define led 13
static CAN_message_t txMsg, rxMsg;



void setup() {
  // put your setup code here, to run once:
  //initScreen1();
  //initScreen2();
  initSerial();
  initCAN();
  
  txMsg.ext = 0; //setting up message with id 270, length 8 and random messages
  txMsg.id = 0x270;
  txMsg.len = 8;
  txMsg.buf[0] = 0x1B;
  txMsg.buf[1] = 0x70;
  txMsg.buf[2] = 0x30;
  txMsg.buf[3] = 0x29;
  txMsg.buf[4] = 0x0;
  txMsg.buf[5] = 0x3A;
  txMsg.buf[6] = 0x10;
  txMsg.buf[7] = 0x10;

  pinMode(led, OUTPUT); digitalWrite(led, HIGH); //setting up leds
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
}

void loop() {
     
  writeCan(txMsg);
  //printCanToSerial(txMsg);
  
  if(readCan(rxMsg)==0) {
    digitalWrite(blueLed, HIGH);  //turnes on blue led whenever a message is recieved.
  }
  
  printCanToSerial(rxMsg);
    delay(100);
  //Serial.println(rxMsg.id, HEX);
  if (rxMsg.id == 0x230) { //test if the id is 230
    Serial.print("Swheel: "); Serial.println(" "); printCanToSerial(rxMsg); digitalWrite(greenLed, HIGH); //print message
  }
  if (rxMsg.id == 0x260) { //test if id is 260
    Serial.print("Motor: "); Serial.println(" "); printCanToSerial(rxMsg); digitalWrite(greenLed, HIGH); //print message
  }

  delay(200);
  
  digitalWrite(blueLed, LOW); //testing of leds on the pcb, seem to be working fine.
  
  delay(200); //delays to make fancy lights and to prevent can from crashing (short codes run main too fast/often)
  
  digitalWrite(greenLed, LOW); //testing of leds
  delay(200);
}
