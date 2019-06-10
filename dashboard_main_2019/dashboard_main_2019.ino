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
int optimalCounter;


static CAN_message_t txMsg, rxMsg;

Adafruit_NeoPixel frontlights(NUM_FRONTLIGHTS, PIN_FRONTLIGHT, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel backlights(NUM_BACKLIGHTS, PIN_BACKLIGHT, NEO_GRBW + NEO_KHZ800);

Adafruit_SharpMem screen1(S1_SCK, S1_MOSI, S1_CS, WIDTH, HEIGHT);

Adafruit_SharpMem screen2(S2_SCK, S2_MOSI, S2_CS, WIDTH, HEIGHT);

enum ORIENTATION { UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 };

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

void initScreen1() {
    screen1.begin();
    screen1.clearDisplay();
     drawBackground(screen1, true);
     
    screen1.refresh();
}

void initScreen2() {
    screen2.begin();
    screen2.clearDisplay();
    drawBackground(screen2, false);

    screen2.refresh();
}

void testdrawchar(void) {
  
  screen1.setTextSize(6);
  screen1.setTextColor(WHITE);
  screen1.setCursor(130,80);
  screen1.cp437(true);

  
    screen1.write('1');
    screen1.write('0');
    screen1.write('/');
    screen1.write('1');
    screen1.write('0');

  screen1.refresh();
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

  initScreen1();

  delay(100);
  initScreen2();


  delay(100);
  char str[] = "hei";
  drawString(screen1, str, 50, 10, 4);
  screen1.refresh();
  //testdrawchar();
}


/*----------------------- MAIN LOOP -----------------------*/

void loop() {
  //screen1.refresh();
  delay(400);
  //screen2.refresh();
  /*delay(500);
  testdrawrect(screen1);
  Serial.print("done 1");
  screen1.clearDisplay();
  testfillrect(screen2);
  Serial.print("done 2\n");
  delay(500);
  //screen1.clearDisplay();/*
//  screen2.clearDisplay();
  //delay(500);
  //drawString(screen1, "screen1", 100, 100, 10);*/
  
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
