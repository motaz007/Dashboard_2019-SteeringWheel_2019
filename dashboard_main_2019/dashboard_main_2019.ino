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

#define PIN_LIGHT_ENABLE     24
#define PIN_BATTERY          25
#define PIN_PROPULTION_POWER 26
#define PIN_RESET            27
#define PIN_BRAKE_ENABLED    28

#define PIN_WIPER            5
#define PIN_HAZARD_LIGHT     6
#define PIN_RACE_SHOW_MODE   7
#define PIN_RESET            8

//#define PIN_MAIN_SWITCH      37

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
int optimalCounter;


static CAN_message_t txMsg, rxMsg;

Adafruit_NeoPixel frontlights(NUM_FRONTLIGHTS, PIN_FRONTLIGHT, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel backlights(NUM_BACKLIGHTS, PIN_BACKLIGHT, NEO_GRBW + NEO_KHZ800);

Adafruit_SharpMem leftScreen(SL_SCK, SL_MOSI, SL_CS, WIDTH, HEIGHT); //leftScreen stygt?

Adafruit_SharpMem rightScreen(SR_SCK, SR_MOSI, SR_CS, WIDTH, HEIGHT);

//enum ORIENTATION { UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 };


uint8_t lap = 0;
uint8_t maxLap = 10;
uint8_t lapTime = 0;
uint8_t sector = 0;
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

  /*frontlights.begin();
  backlights.begin();
  startUpLights(frontlights, backlights);*/
  char str1[] = "Laps";
  char str2[] = "taken";

  initScreen(leftScreen, LEFTSCREEN);
  initText(leftScreen, LEFTSCREEN);
  
  delay(500);
  initScreen(rightScreen, RIGHTSCREEN);
  initText(rightScreen, RIGHTSCREEN);
  
  drawSector(leftScreen, sector);
  drawLapCount(leftScreen, lap, maxLap);
  drawLapTime(leftScreen, lapTime);

  drawTime(leftScreen, 2000);

  drawSpeed(rightScreen, 2, 150);
  drawGear(rightScreen, '2');

  drawLightIcon(rightScreen, true);
  

  rightScreen.refresh();
  leftScreen.refresh();
  
}


/*----------------------- MAIN LOOP -----------------------*/

void loop() {
  lapTime++;
  lap++;
  sector++;
  if (lap >10) {
    lap=1;
  } else if (sector > 5) {
    sector = 1;
  }
  drawLapCount(leftScreen, lap, maxLap);

  drawLapTime(leftScreen, lapTime);
  drawTime(leftScreen, 200+lapTime);

  drawSector(leftScreen, sector);

  drawSpeed(rightScreen, 4.8+sector, 5.2+sector);

  switch(sector) {
    case 1:
      drawGear(rightScreen, '1');
      drawRaceMode(rightScreen, false);
      break;
    case 2:
      drawGear(rightScreen, 'N');
      drawRaceMode(rightScreen, true);
      break;
    case 3:
      drawGear(rightScreen, '2');
      break;
  }

  drawVoltageValue(rightScreen, 47.5+lap*0.1);
  drawCurrentValue(rightScreen, 12.4+lap*1.1, 13.5+lap*1.1);
  drawPowerConsumption(rightScreen, 5.4+lap*4.1, 5.5+lap*4.1);

  rightScreen.refresh();
  leftScreen.refresh();
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
  // add later
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
