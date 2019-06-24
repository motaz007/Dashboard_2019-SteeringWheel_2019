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
// RIGHTSCREEN
#define SR_SCK  13
#define SR_MOSI 11
#define SR_CS   10

// LEFT SCREEN
#define SL_SCK  32
#define SL_MOSI 21
#define SL_CS   31

// LIGHTS
#define PIN_BACKLIGHT   35
#define PIN_FRONTLIGHT   36


// BUTTONS

//Right
#define PIN_WIPER            5
#define PIN_HAZARD_LIGHT     6
#define PIN_RACE_SHOW_MODE   7
#define PIN_RESET            8

//left
#define PIN_LIGHT_ENABLE     24
#define PIN_BATTERY          25
#define PIN_PROPULTION_POWER 26
#define PIN_BLANK            27
#define PIN_BRAKE_ENABLED    28


// LED's on PCB for CAN
#define PIN_CAN_RX_LED 23
#define PIN_CAN_TX_LED 22

/*-------------------------- VARIABLES -------------------------*/

bool brakeON = false;
bool regenBrakeON = false;
bool hazardLightON = false;
bool hazardLightRunning = false;
bool lightON = true;
bool raceModeON = false;
bool windowWiperON = false;
uint8_t brakeVal = 0;
uint8_t optimalCounter;


static CAN_message_t txMsg, rxMsg;

Adafruit_NeoPixel frontlights(NUM_FRONTLIGHTS, PIN_FRONTLIGHT, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel backlights(NUM_BACKLIGHTS, PIN_BACKLIGHT, NEO_GRBW + NEO_KHZ800);

Adafruit_SharpMem leftScreen(SL_SCK, SL_MOSI, SL_CS, WIDTH, HEIGHT); 

Adafruit_SharpMem rightScreen(SR_SCK, SR_MOSI, SR_CS, WIDTH, HEIGHT);

/*------------------------- FUCTIONS -------------------------*/


// BUTTONS

#define PIN_WIPER            5
#define PIN_HAZARD_LIGHT     6
#define PIN_RACE_MODE   7
#define PIN_RESET            8

#define PIN_LIGHT_ENABLE     24
#define PIN_BATTERY          25                                                             //not used?
#define PIN_PROPULTION_POWER 26                                                             //not used?
#define PIN_BLANK            27
#define PIN_BRAKE_ENABLED    28

void initPins() {

    pinMode(PIN_WIPER, INPUT_PULLUP);                                                       //interrupt wiper
    attachInterrupt(digitalPinToInterrupt(PIN_WIPER),   wiper_ISR, FALLING);

    pinMode(PIN_HAZARD_LIGHT, INPUT_PULLUP);                                                //interrupt hazzard light
    attachInterrupt(digitalPinToInterrupt(PIN_HAZARD_LIGHT), hazardButton_ISR, FALLING);
    
    pinMode(PIN_RACE_MODE, INPUT_PULLUP);                                                   //interrupt race mode
    attachInterrupt(digitalPinToInterrupt(PIN_RACE_MODE), raceMode_ISR(), FALLING);
    
    pinMode(PIN_RESET, INPUT_PULLUP);                                                       //interrupt reset
    attachInterrupt(digitalPinToInterrupt(PIN_RESET), resetButton_ISR(), FALLING);

    pinMode(PIN_LIGHT_ENABLE, INPUT_PULLUP);                                                //interrupt light
    attachInterrupt(digitalPinToInterrupt(PIN_LIGHT_ENABLE), raceShowMode_ISR, FALLING);

    pinMode(PIN_BLANK, INPUT_PULLUP);                                                       //interrupt blank
    attachInterrupt(digitalPinToInterrupt(PIN_BLANK), blankButton_ISR(), FALLING);

    
    pinMode(PIN_BRAKE_ENABLED, INPUT_PULLUP);                                                //interrupt brake
    attachInterrupt(digitalPinToInterrupt(PIN_BRAKE_ENABLED), brakeButtonChanged_ISR, CHANGE);
}



/*------------------------- SETUP -------------------------*/
void setup() {
  //clockSpeedHigh(debug);
  initSerial();
  initCAN();
  initPins();
    txMsg.ext = 0;
    
  txMsg.id = 0x270;               //setting id
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
  
  initScreen(leftScreen, LEFTSCREEN);
  initText(leftScreen, LEFTSCREEN);
  
  delay(500);
  initScreen(rightScreen, RIGHTSCREEN);
  initText(rightScreen, RIGHTSCREEN);

}


/*----------------------- MAIN LOOP -----------------------*/

void loop() {

  delay(1000);
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
  if (debug == true) 
  {
    debug = false;
  }
  debug = true;
}

void lightOnOff_ISR(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  
  if (interrupt_time - last_interrupt_time > 100){
    int state = digitalRead(PIN_HAZARD_LIGHT);
    if(state == LOW && lightON == false){
      lightON = true;
      if(raceModeON){
        raceMode(frontlights, backlights);
      }else{
        startUpLights(frontlights, backlights);
      }
    }else{
      turnOffStrip(frontlights, 0, NUM_FRONTLIGHTS);
      turnOffStrip(backlights, 0, NUM_BACKLIGHTS);
      lightON = false;
    }
  last_interrupt_time = interrupt_time;
  }
}

void windowWiper_ISR(){
  static unsigned long last_interrupt_time = 0;
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

void reset_ISR(){
  
}


/*---------------- OTHER FUNCTIONS ---------------------*/

void sWheelCan(){
  if (rxMsg.id == 0x230){ 
    brakeVal = rxMsg.buf[2];
    if(brakeVal > 0 && regenBrakeON == false){  // BRAKELIGHTS
      brakeLights(backlights, BRIGHTNESS_BACK_BRAKE);
      regenBrakeON = true;
    }else if(brakeVal == 0 && regenBrakeON == true){
      brakeLights(backlights, BRIGHTNESS_BACK);
      regenBrakeON = false;
    }
    if(bitRead(rxMsg.buf[1],1)){ // LEFT BLINK //bitRead(rxMsg.buf[1],1) rxMsg.buf[1] &= (1<<1)
      blinkLights(frontlights, backlights, true, raceModeON);
    }
    if(bitRead(rxMsg.buf[1],2)){ // RIGHT BLINK  //bitRead(rxMsg.buf[1],2) rxMsg.buf[1] &= (1<<2)
      blinkLights(frontlights, backlights, false, raceModeON);
    }
    if(bitRead(rxMsg.buf[1],3)){ //CC-button, counter for optimal acceleration
      static unsigned long last_interrupt_time = 0;
      unsigned long interrupt_time = millis();
      if (interrupt_time - last_interrupt_time > 200){
        optimalCounter++;
        if(optimalCounter > OPTIMALCOUNTER_MAX){
          optimalCounter = 0;
        }
        last_interrupt_time = interrupt_time;
      }
    }
    if(bitRead(rxMsg.buf[1],4)){ //OptimalCurrent
      
    }
    if(bitRead(rxMsg.buf[1],5)){ //Lap
      
    }
    if(bitRead(rxMsg.buf[1],6)){ // Horn
      // HORN ON
    }
    if(bitRead(rxMsg.buf[1],7)){ //OptimalBrake
      
    }
  }
}
