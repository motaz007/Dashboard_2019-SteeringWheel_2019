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
// RIGHT SCREEN
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
  #define PIN_WIPER_BUTTON     5
  #define PIN_HAZARD           6
  #define PIN_RACE_MODE        7
  #define PIN_RESET            8
  
  #define PIN_LIGHT_BUTTON     24
  #define PIN_BATTERY          25                                                             //not used
  #define PIN_PROPULTION_POWER 26                                                             //not used
  #define PIN_BLANK            27
  #define PIN_BRAKE            28

<<<<<<< HEAD
#define PIN_JENS_1 15                                                                       //nye navn ofc
=======
#define PIN_HORN 15
>>>>>>> 4b7a2bd651859a93386bbdae6400632981a6beb6
#define PIN_JENS_2 16

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



Adafruit_NeoPixel frontlights(NUM_FRONTLIGHTS, PIN_FRONTLIGHT, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel backlights(NUM_BACKLIGHTS, PIN_BACKLIGHT, NEO_GRBW + NEO_KHZ800);

Adafruit_SharpMem leftScreen(SL_SCK, SL_MOSI, SL_CS, WIDTH, HEIGHT); 
Adafruit_SharpMem rightScreen(SR_SCK, SR_MOSI, SR_CS, WIDTH, HEIGHT);

/*------------------------- FUCTIONS -------------------------*/

void initPins() {

    pinMode(PIN_WIPER_BUTTON, INPUT_PULLUP);                                                       //init wiper with interrupt
    attachInterrupt(digitalPinToInterrupt(PIN_WIPER_BUTTON), wiper_ISR, FALLING);

    pinMode(PIN_HAZARD, INPUT_PULLUP);                                                      //init hazzard light with interrupt
    attachInterrupt(digitalPinToInterrupt(PIN_HAZARD), hazard_ISR, FALLING);

    pinMode(PIN_RACE_MODE, INPUT_PULLUP);                                                   //init race mode with interrupt
    attachInterrupt(digitalPinToInterrupt(PIN_RACE_MODE), raceMode_ISR, FALLING);

    pinMode(PIN_RESET, INPUT_PULLUP);                                                       //ini reset button with interrupt
    attachInterrupt(digitalPinToInterrupt(PIN_RESET), reset_ISR, FALLING);

    pinMode(PIN_LIGHT_BUTTON, INPUT_PULLUP);                                                //init light button with interrupt
    attachInterrupt(digitalPinToInterrupt(PIN_LIGHT_BUTTON), lights_ISR, FALLING);

    pinMode(PIN_BLANK, INPUT_PULLUP);                                                       //init blank button with interrupt
    attachInterrupt(digitalPinToInterrupt(PIN_BLANK), blank_ISR, FALLING);

    pinMode(PIN_BRAKE, INPUT_PULLUP);                                                       //init brake pedal sensor (button) with interrupt
    attachInterrupt(digitalPinToInterrupt(PIN_BRAKE), brake_ISR, CHANGE);

    pinMode(PIN_HORN, OUTPUT);
    

    if(debug) {
      Serial.println("all buttons initialized with interrupt");
    }
}

/*------------------------- SETUP -------------------------*/
void setup() {
  //clockSpeedHigh(debug);
  initSerial();
  initCAN();
  initCanMessage(txMsg, 3); 

  initLights(frontlights, backlights);
  showLights(frontlights, backlights);
  
  initScreen(leftScreen, LEFTSCREEN);
  initScreen(rightScreen, RIGHTSCREEN);

  initPins();                                                                               //initPins needs to be last to avoid malfunction, not sure what goes wrong
}


/*----------------------- MAIN LOOP -----------------------*/

void loop() {
  
  readCan(rxMsg, sWheelMsg);
  updateScreen(leftScreen, LEFTSCREEN);
  updateScreen(rightScreen, RIGHTSCREEN);
  sWheelCan();
  //printCanToSerial(sWheelMsg, true);
  delay(100);
}

/*----------------------- ISR FUCTIONS -----------------------*/

void wiper_ISR(){
  static unsigned long last_interrupt_time;
  unsigned long interrupt_time = millis();

  delay(50);
  if (interrupt_time - last_interrupt_time > 500){
    delay(50);
    
    Serial.println("wiper pressed"); //if debug
    int state = digitalRead(PIN_HAZARD);
    if(state == LOW && windowWiperON == false) {
      windowWiperON = true;
    } else {
      windowWiperON = false;
    }
  last_interrupt_time = interrupt_time;
  }
}

void hazard_ISR(){
  static unsigned long last_interrupt_time;
  unsigned long interrupt_time = millis();

  delay(50);
  if (interrupt_time - last_interrupt_time > 500){
    delay(50);
      Serial.println("hazzard pressed"); //if debug
      int state = digitalRead(PIN_HAZARD);
      if( state == LOW && hazardLightON == false){
        hazardLightON = true;
      }else{
        hazardLightON = false;
      }
      last_interrupt_time = interrupt_time;
     }
}


void raceMode_ISR(){
  static unsigned long last_interrupt_time;
  unsigned long interrupt_time = millis();

    delay(50);
  if (interrupt_time - last_interrupt_time > 500){
    delay(50);
    
    Serial.println("raceMode pressed"); //if debug
    int state = digitalRead(PIN_RACE_MODE);
    //Serial.println(state);
    if(state == HIGH && raceModeON == false){
      raceModeON = true;
      raceLights(frontlights, backlights);
      
    }else if(state == HIGH && raceModeON == true){
      raceModeON = false;
      showLights(frontlights, backlights);
    }
  last_interrupt_time = interrupt_time;
  }
}

void reset_ISR(){
  static unsigned long last_interrupt_time;
      unsigned long interrupt_time = millis();
     delay(50);
  if (interrupt_time - last_interrupt_time > 500){
    delay(15);
    //nullstill diverse
    Serial.println("reset pressed"); //if debug
    }
  last_interrupt_time = interrupt_time;
}


void lights_ISR(){
  static unsigned long last_interrupt_time;
  unsigned long interrupt_time = millis();

 
    delay(50);
  if (interrupt_time - last_interrupt_time > 500){
    delay(50); 
    Serial.println("lights pressed"); //if debug
    int state = digitalRead(PIN_HAZARD);
    if(state == LOW && lightON == false){
      lightON = true;
      if(raceModeON){
        raceLights(frontlights, backlights);
      }else{
        showLights(frontlights, backlights);
      }
    }else{
      turnOffStrip(frontlights, 0, NUM_FRONTLIGHTS);
      turnOffStrip(backlights, 0, NUM_BACKLIGHTS);
      lightON = false;
    }
  last_interrupt_time = interrupt_time;
  }
}


void blank_ISR(){
  static unsigned long last_interrupt_time;
  unsigned long interrupt_time = millis();
    delay(50);
  if (interrupt_time - last_interrupt_time > 500){
    delay(50);
  Serial.println("blank aka debug pressed"); //if debug
  
  if (debug == true) 
  {
    debug = false;
  }
  debug = true;
      }
  last_interrupt_time = interrupt_time;
}

void brake_ISR() { //interrupt function for brake light
    // this button is reversed (physically, normally open)

  static unsigned long last_interrupt_time;
  unsigned long interrupt_time = millis();

    delay(50);
  if (interrupt_time - last_interrupt_time > 200){
    delay(50);
    int state = digitalRead(PIN_BRAKE);

    if (state == HIGH && regenBrakeON == false) {  // RISING -- is unpressed
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

/*---------------- OTHER FUNCTIONS ---------------------*/

void sWheelCan()
{
    brakeVal = sWheelMsg.buf[2];
    if (brakeVal > 0 && regenBrakeON == false) {  // BRAKELIGHTS
      brakeLights(backlights, BRIGHTNESS_BACK_BRAKE);
      regenBrakeON = true;
<<<<<<< HEAD
    } else if (brakeVal == 0 && regenBrakeON == true) {
=======
    }else if(brakeVal == 0 && regenBrakeON == true && brakeON == false){
>>>>>>> 4b7a2bd651859a93386bbdae6400632981a6beb6
      brakeLights(backlights, BRIGHTNESS_BACK);
      regenBrakeON = false;
    }
    if (bitRead(sWheelMsg.buf[1],1)) { // LEFT BLINK //bitRead(rxMsg.buf[1],1) rxMsg.buf[1] &= (1<<1)
      blinkLights(frontlights, backlights, true, raceModeON);
    }
    if (bitRead(sWheelMsg.buf[1],2)) { // RIGHT BLINK  //bitRead(rxMsg.buf[1],2) rxMsg.buf[1] &= (1<<2)
      blinkLights(frontlights, backlights, false, raceModeON);
    }
<<<<<<< HEAD
    if (bitRead(sWheelMsg.buf[1],3)) { //CC-button, counter for optimal acceleration
     // static unsigned long last_interrupt_time = 0;
      unsigned long interrupt_time = millis();
      if (interrupt_time - last_interrupt_time > 200){
        optimalCounter++;
        if(optimalCounter > OPTIMALCOUNTER_MAX){
          optimalCounter = 0;
        }
        last_interrupt_time = interrupt_time;
      }
=======
    if(bitRead(sWheelMsg.buf[1],3)){ //CC-button, counter for optimal acceleration
      
>>>>>>> 4b7a2bd651859a93386bbdae6400632981a6beb6
    }
    if (bitRead(sWheelMsg.buf[1],4)) { //OptimalCurrent
      
    }
    if (bitRead(sWheelMsg.buf[1],5)) { //Lap
      
    }
<<<<<<< HEAD
    if (bitRead(sWheelMsg.buf[1],6)) { // Horn
      // HORN ON
=======
    if(bitRead(sWheelMsg.buf[1],6)){ // Horn
      digitalRead(PIN_HORN, HIGH);
    }else{
      digitalRead(PIN_HORN, LOW);
>>>>>>> 4b7a2bd651859a93386bbdae6400632981a6beb6
    }
    if (bitRead(sWheelMsg.buf[1],7)) { //OptimalBrake
      
    }
}
