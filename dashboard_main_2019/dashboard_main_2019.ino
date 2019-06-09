/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * Code for the dashboard of the 2019 car
 */

/*------------------ Include Headerfiles ------------------*/
#include "can.h"
#include "graphics.h"
#include "helpers.h"
#include "lights.h"

/*-------------------------- DEBUG -------------------------*/
bool debug = true;


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

bool brakeON = false;
bool regenBrakeON = false;
bool hazardLightON = false;
bool hazardLightRunning = false;
bool lightON = true;
bool raceModeON = false;
bool windowWiperON = false;
int brakeVal = 0;


static CAN_message_t txMsg, rxMsg;

Adafruit_NeoPixel frontlights(NUM_FRONTLIGHTS, PIN_FRONTLIGHT, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel backlights(NUM_BACKLIGHTS, PIN_BACKLIGHT, NEO_GRBW + NEO_KHZ800);


/*------------------------- FUCTIONS -------------------------*/

void initPins() {
    // Dashboard buttons
    pinMode(PIN_BRAKE_ENABLED, INPUT_PULLUP);  // push button (at pedal)
    attachInterrupt(digitalPinToInterrupt(PIN_BRAKE_ENABLED), brakeButtonChanged_ISR, CHANGE); //ISR for brake

    pinMode(PIN_HAZARD_LIGHT, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(PIN_HAZARD_LIGHT), hazardButtonChanged_ISR, FALLING);

    pinMode(PIN_LIGHT_ENABLE, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_LIGHT_ENABLE), raceShowMode_ISR, FALLING);
    //add other buttons
}

/*------------------------- SETUP -------------------------*/
void setup() {
  //clockSpeedHigh(debug);
  initSerial();
  initCAN();
  initPins();
    txMsg.ext = 0;
    
  txMsg.id = 0x270;                //setting id
  txMsg.len = 8;                  //setting length
  txMsg.buf[0] = 0x00;            //setting values
  txMsg.buf[1] = 0x00;
  txMsg.buf[2] = 0x00;
  txMsg.buf[3] = 0x00;
  txMsg.buf[4] = 0x00;
  txMsg.buf[5] = 0x00;
  txMsg.buf[6] = 0x00;
  txMsg.buf[7] = 0x00;

  frontlights.begin();
  backlights.begin();
  startUpLights(frontlights, backlights);

}




/*----------------------- MAIN LOOP -----------------------*/

void loop() {
  
  if(Serial.available() > 0 )  {  // MARKUS TEST 2. GEAR
    int incomingData= Serial.read(); 
    switch(incomingData) { 
        case 48:
           txMsg.buf[7] = 0;
           break;
      
        case 49:
           txMsg.buf[7] = 1;
           break;

        case 50:
           txMsg.buf[7] = 2;
           break;
           
        default:
           
           break;
   }
  }

  //Serial.println(raceModeON);
  writeCan(txMsg);
  readCan(rxMsg);
  //printCanToSerial(rxMsg, debug);  
  hazardLightRunning = hazardLights(frontlights, backlights, hazardLightON, raceModeON);
  sWheelCan();
  delay(50); //needs delay because CANbus crash if everybody sends stuff all the time. Long codes that takes time runnin don't need this 
  //Serial.println(brakeVal);

  
}

/*----------------------- ISR FUCTIONS -----------------------*/

void brakeButtonChanged_ISR() { //interrupt function for brake light
    // this button is reversed

    static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  
  if (interrupt_time - last_interrupt_time > 100)
     {
    delay(10);
    int state = digitalRead(PIN_BRAKE_ENABLED);

    if (state == HIGH) {  // RISING -- is unpressed
        // brake lights off 
        brakeON = false;
        brakeLights(backlights, brakeON);
        Serial.println("  NOT BRAKING  ");
        txMsg.buf[0] = 0x01;
    }
    else if(state == LOW){
        // brake lights on
        brakeON = true;
        brakeLights(backlights, brakeON);
        Serial.println("  BRAKING  ");
        txMsg.buf[0] = 0x00;

    }
    last_interrupt_time = interrupt_time;
     }
} 

void hazardButtonChanged_ISR(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  
  if (interrupt_time - last_interrupt_time > 100)
     {
      int state = digitalRead(PIN_HAZARD_LIGHT);
      if( state == LOW && hazardLightON == false){
        hazardLightON = true;
      }else{
        hazardLightON = false;
      }
      last_interrupt_time = interrupt_time;
     }
}

void blankButton_ISR(){
  // add later
}

void lightOnOff_ISR(){
  unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  
  if (interrupt_time - last_interrupt_time > 100){
    int state = digitalRead(PIN_HAZARD_LIGHT);
    if(state == LOW && lightON == false){
      lightON = true;
      startUpLights(frontlights, backlights);
    }else{
      turnOffStrip(frontlights, 0, NUM_FRONTLIGHTS);
      turnOffStrip(backlights, 0, NUM_BACKLIGHTS);
      lightON = false;
    }
  last_interrupt_time = interrupt_time;
  }
}

void windowWiper_ISR(){
  unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  
  if (interrupt_time - last_interrupt_time > 100){
    int state = digitalRead(PIN_HAZARD_LIGHT);
    if(state == LOW && windowWiperON == false){
      windowWiperON = true;
    }else{
      windowWiperON = false;
    }
  last_interrupt_time = interrupt_time;
  }
}

void raceShowMode_ISR(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  
  if (interrupt_time - last_interrupt_time > 2000){

    int state = digitalRead(PIN_HAZARD_LIGHT);
    Serial.println(state);
    if(state == HIGH && raceModeON == false){
      raceModeON = true;
      raceMode(frontlights, backlights);
      
    }else if(state == HIGH && raceModeON == true){
      raceModeON = false;
      startUpLights(frontlights, backlights);
    }
  last_interrupt_time = interrupt_time;
  }
}


/*---------------- OTHER FUNCTIONS ---------------------*/

void sWheelCan(){
  if (rxMsg.id == 0x230){ 
    brakeVal = rxMsg.buf[2];
    if(brakeVal > 0 && regenBrakeON == false){  // BLINKLIGHTS
      brakeLights(backlights, BRIGHTNESS_BACK_BRAKE);
      regenBrakeON = true;
    }else if(brakeVal == 0 && regenBrakeON == true){
      brakeLights(backlights, BRIGHTNESS_BACK);
      regenBrakeON = false;
    }
    if(bitRead(rxMsg.buf[1],1)){ // LEFT //bitRead(rxMsg.buf[1],1) rxMsg.buf[1] &= (1<<1)
      blinkLights(frontlights, backlights, true, raceModeON);
    }
    if(bitRead(rxMsg.buf[1],2)){ // RIGHT  //bitRead(rxMsg.buf[1],2) rxMsg.buf[1] &= (1<<2)
      blinkLights(frontlights, backlights, false, raceModeON);
    }
  }
}
