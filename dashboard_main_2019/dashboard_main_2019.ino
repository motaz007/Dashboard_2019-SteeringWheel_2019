/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * Code for the dashboard of the 2019 car
 */

/*------------------ Include Headerfiles ------------------*/
#include "can.h"
#include "graphics.h"
#include "helpers.h"
#include "lights.h"
//test

/*-------------------------- PINS -------------------------*/

/* COMM PARAMETERS */
// SCREEN 1 //endre til left eller right
#define S1_SCK  13
#define S1_MOSI 11
#define S1_CS   10

// SCREEN 2
#define S2_SCK  32
#define S2_MOSI 21
#define S2_CS   31

// LIGHTS
#define PIN_BACKLIGHT   35
#define PIN_FRONTLIGHT   36

// BUTTONS
#define PIN_BRAKE_ENABLED    26
#define PIN_LIGHT_ENABLE     28
#define PIN_MAIN_SWITCH      37
#define PIN_HAZARD_LIGHT     16
#define PIN_RESET            30

// LED's on PCB for CAN
#define PIN_CAN_RX_LED 23
#define PIN_CAN_TX_LED 22

static CAN_message_t txMsg, rxMsg;

/*-------------------------- DEBUG? -------------------------*/
bool debug = false;


/*------------------------- FUCTIONS -------------------------*/

void initPins() {
    // Dashboard buttons
    pinMode(PIN_BRAKE_ENABLED, INPUT_PULLUP);  // push button (at pedal)
    attachInterrupt(digitalPinToInterrupt(PIN_BRAKE_ENABLED), brakeButtonChanged_ISR, CHANGE); 
    
}

Adafruit_NeoPixel light(13, 36, NEO_GRBW + NEO_KHZ800);

/*------------------------- SETUP -------------------------*/
void setup() {
  // put your setup code here, to run once:

  initSerial(); //inizializing
  initCAN();
  initPins();
    txMsg.ext = 0;
    
  txMsg.id = 0x270;                //setting id
  txMsg.len = 8;                  //setting length
  txMsg.buf[0] = 0x00;            //setting values
  txMsg.buf[1] = 0x70;
  txMsg.buf[2] = 0x30;
  txMsg.buf[3] = 0x29;
  txMsg.buf[4] = 0x0;
  txMsg.buf[5] = 0x3A;
  txMsg.buf[6] = 0x10;
  txMsg.buf[7] = 0x10;
  startUpSwheelLight(light);
}



/*----------------------- MAIN LOOP -----------------------*/

void loop() {
  writeCan(txMsg);
  readCan(rxMsg);

  if(rxMsg.id == 0x230) {
    brakeButtonChanged_ISR();
  }
  Serial.println("loop");
 // printCanToSerial(rxMsg);
  delay(2000); 
  
}

/*----------------------- ISR FUCTIONS -----------------------*/

void brakeButtonChanged_ISR() { 
    // this button is reversed
    int state = digitalRead(PIN_BRAKE_ENABLED);
    Serial.println("isr");
    if (state == HIGH || rxMsg.buf[3]>0) {  // RISING -- is unpressed
        // brake lights on

        //setStripBrightness(backLights, 100);
      
        Serial.print("  NOT BRAKING  ");
        //txMsg.buf[0] = 0x01;
        
    }
    else {
        // brake lights off

        //setStripBrightness(backLights, 40);
        Serial.println("  BRAKING  ");
        //txMsg.buf[0] = 0x00;
    }
} 
