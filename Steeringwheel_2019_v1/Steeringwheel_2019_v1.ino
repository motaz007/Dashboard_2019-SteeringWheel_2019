// MUST BE COMPILED AT 120 MHZ!!!


#include "can.h"
#include "lights.h"
#include "helpers.h"
#include <Snooze.h>


SnoozeBlock Config;

//----- DEBUG -----//
bool debug = false;

// LIGHT
#define PIN_SWHEEL_LIGHT    16

// BUTTONS
#define PIN_THROTTLE        21
#define PIN_REGEN           20
#define PIN_DEADMAN_SWITCH  15
#define PIN_BLINKER_L       17
#define PIN_BLINKER_R       18
#define PIN_LAP_BUTTON      35
#define PIN_HORN            36
#define PIN_CC              33
#define PIN_OPTIMAL_CURRENT 34
#define PIN_OPTIMAL_BRAKE   37
#define PIN_BLANK           38

// BITVALUE BUTTONS
#define deadmanSwitch   1
#define leftBlink       (1<<1)
#define rightBlink      (1<<2)
#define cc              (1<<3)
#define optimalCurrent  (1<<4)
#define lap             (1<<5)
#define horn            (1<<6)
#define optimalBrake    (1<<7)
#define blank           1       // New byte

bool ccActive = false;
bool optimalCurrentActive = false;
bool optimalBrakeActive = false;
bool blinkLED = false;

static volatile uint8_t buttons, buttons2;

int n = 0;

static CAN_message_t txMsg, rxMsg;

Adafruit_NeoPixel swheelLights(14, PIN_SWHEEL_LIGHT, NEO_GRBW + NEO_KHZ800);



void initPins() {
    // Steering wheel buttons
    pinMode(PIN_THROTTLE, INPUT);  // pot 

    pinMode(PIN_REGEN, INPUT); // pot

    pinMode(PIN_DEADMAN_SWITCH, INPUT_PULLUP);  // push button
    attachInterrupt(digitalPinToInterrupt(PIN_DEADMAN_SWITCH), deadmanSwitchChanged_ISR, CHANGE);

    pinMode(PIN_LAP_BUTTON, INPUT_PULLUP);  // push button, debounced
    attachInterrupt(digitalPinToInterrupt(PIN_LAP_BUTTON), resetLapTimeAndIncrementLapCount_ISR, CHANGE);

    pinMode(PIN_BLINKER_L, INPUT_PULLUP);  // push button
    attachInterrupt(digitalPinToInterrupt(PIN_BLINKER_L), leftBlinkerChanged_ISR, CHANGE);

    pinMode(PIN_BLINKER_R, INPUT_PULLUP);  // push button
    attachInterrupt(digitalPinToInterrupt(PIN_BLINKER_R), rightBlinkerChanged_ISR, CHANGE);

    pinMode(PIN_HORN, INPUT_PULLUP);  // push button, debounced
    attachInterrupt(digitalPinToInterrupt(PIN_HORN), hornChanged_ISR, CHANGE);

    pinMode(PIN_CC, INPUT_PULLUP);  // push button, debounced
    attachInterrupt(digitalPinToInterrupt(PIN_CC), ccButtonPressed_ISR, FALLING);

    pinMode(PIN_OPTIMAL_CURRENT, INPUT_PULLUP);  // push button, debounced
    attachInterrupt(digitalPinToInterrupt(PIN_OPTIMAL_CURRENT), optimalCurrentButtonChanged_ISR, FALLING);

    pinMode(PIN_OPTIMAL_BRAKE, INPUT_PULLUP);  // push button, debounced
    attachInterrupt(digitalPinToInterrupt(PIN_OPTIMAL_BRAKE), optimalBrakeButtonChanged_ISR, FALLING);

    pinMode(PIN_BLANK, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_BLANK), blankButtonChanged_ISR, CHANGE);
}

void setup() {
  initCAN();
  initPins();
  startUpSwheelLight(swheelLights);
  txMsg.id = 0x230;
  txMsg.len = 8;
  
  txMsg.buf[1] = 0x00;
  txMsg.buf[2] = 0x00;
  txMsg.buf[3] = 0x00;
  txMsg.buf[4] = 0x00;
  txMsg.buf[5] = 0x00;
  txMsg.buf[6] = 0x00;
  txMsg.buf[7] = 0x00;
  
  clockSpeed2Mhz(debug);

}

void loop(){  
  txMsg.buf[0] = buttons2;  
  txMsg.buf[1] = buttons;
  txMsg.buf[2] = readRegen(PIN_REGEN, &buttons , ccActive, optimalBrakeActive);
  txMsg.buf[3] = readThrottle(PIN_THROTTLE, &buttons, ccActive, optimalCurrentActive);
  readCan(rxMsg);
  writeCan(txMsg);
  delay(50);
}


//----- ISR -----//

void resetLapTimeAndIncrementLapCount_ISR() {
     static unsigned long last_interrupt_time = 0;
     unsigned long interrupt_time = millis();
     // If interrupts come faster than 10ms, assume it's a bounce and ignore
      if (interrupt_time - last_interrupt_time > 10)
     {
      int state = digitalRead(PIN_LAP_BUTTON);
      
      if (state == HIGH) {  // RISING -- is unpressed
          buttons &= ~lap;
          Serial.print("  LAP UNPRESSED  ");
      }
      if (state == LOW && interrupt_time - last_interrupt_time > 200) {
          buttons |= lap;
          Serial.print("  LAP PRESSED  ");
      }
     }
    last_interrupt_time = interrupt_time;
}

void deadmanSwitchChanged_ISR() {
    int state = digitalRead(PIN_DEADMAN_SWITCH);

    if (state == HIGH) {  // RISING -- is unpressed

        buttons &= ~deadmanSwitch;
        clockSpeed120Mhz(debug); // Raising clockspeed to be able to change lights
        sWheelError(swheelLights);
        clockSpeed2Mhz(debug);
        Serial.print("  DMS UNPRESSED!  ");
    }
    else {  
        buttons |= deadmanSwitch;
        clockSpeed120Mhz(debug); // Raising clockspeed to be able to change lights
        sWheelOK(swheelLights);
        clockSpeed2Mhz(debug);
        Serial.print("  DMS PRESSED!  ");
    }
}

void leftBlinkerChanged_ISR() {
    int state = digitalRead(PIN_BLINKER_L);
    int leftBlinkerPressed;
    
    if (state == HIGH && leftBlinkerPressed == LOW) {  // RISING -- is unpressed
        leftBlinkerPressed = false;
        buttons &= ~leftBlink;
        Serial.print("  BLINK L UNPRESSED  ");
    }
    else {
        leftBlinkerPressed = true;
        buttons |= leftBlink;
        Serial.print("  BLINK L PRESSED  ");
    }
}

void rightBlinkerChanged_ISR() {
    int state = digitalRead(PIN_BLINKER_R);
    int rightBlinkerPressed;

    if (state == HIGH && rightBlinkerPressed == LOW) {  // RISING -- is unpressed
        rightBlinkerPressed = false;
        // to make sure lights are turned off when button is depressed
        buttons &= ~rightBlink;
        Serial.print("  BLINK R UNPRESSED  ");
    }
    else {
        rightBlinkerPressed = true;

        buttons |= rightBlink;
        Serial.print("  BLINK R PRESSED  ");
    }
}

void hornChanged_ISR() {
    int state = digitalRead(PIN_HORN);

    if(state == HIGH){
      buttons &= ~horn;
      Serial.print(" HORN PRESSED ");
    }else{
      buttons |= horn;
      Serial.print(" HORN UNPRESSED ");
    }
  delay(10);
}

void ccButtonPressed_ISR() {  // CC = Cruise Control
     static unsigned long last_interrupt_time = 0;
     unsigned long interrupt_time = millis();
     // If interrupts come faster than 10ms, assume it's a bounce and ignore
     if (interrupt_time - last_interrupt_time > 10)
     {
        if(ccActive){
            buttons &= ~cc;
            ccActive = false;
            Serial.print(" CC OFF ");
        }else if (!optimalCurrentActive){
            buttons |= cc;
            ccActive = true;
            Serial.print(" CC ON ");
        }
     }
     last_interrupt_time = interrupt_time;
}

void optimalCurrentButtonChanged_ISR() {
     static unsigned long last_interrupt_time = 0;
     unsigned long interrupt_time = millis();
     // If interrupts come faster than 10ms, assume it's a bounce and ignore
     if (interrupt_time - last_interrupt_time > 10)
     {
        if (optimalCurrentActive) {  // RISING -- is unpressed
            buttons &= ~optimalCurrent;
            optimalCurrentActive = false;
            Serial.print("  OPT-CURR OFF  ");
        }
        else if(!ccActive){
            buttons |= optimalCurrent;
            optimalCurrentActive = true;
            Serial.print("  OPT-CURR ON  ");
        }
   }
   last_interrupt_time = interrupt_time;
}

void optimalBrakeButtonChanged_ISR() {
     static unsigned long last_interrupt_time = 0;
     unsigned long interrupt_time = millis();
     // If interrupts come faster than 10ms, assume it's a bounce and ignore
     if (interrupt_time - last_interrupt_time > 10)
     {
        if (optimalBrakeActive) {  // RISING -- is unpressed
            buttons &= ~optimalBrake;
            optimalBrakeActive = false;
            Serial.print("  OPT-BRAKE OFF  ");
        }
        else {
            buttons |= optimalBrake;
            optimalBrakeActive = true;
            Serial.print("  OPT-BRAKE ON  ");
        }
     }
   last_interrupt_time = interrupt_time;
}

void blankButtonChanged_ISR() {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
        int state = digitalRead(PIN_BLANK);

        if (state == HIGH) {  // RISING -- is unpressed
            buttons2 &= ~blank;
            Serial.println(" BLANK UNPRESSED! ");
        }
        else {
            buttons2 |= blank;
            Serial.print(" BLANK PRESSED ");
        }
    last_interrupt_time = interrupt_time;

    Serial.print("  BLANK  ");
}

void brakeEnabled(){
  if (txMsg.buf[2] != 0) {
        ccActive = false;
        buttons &= ~cc;
        optimalCurrentActive = false;
        buttons &= ~optimalCurrent;
    }
   else if (ccActive){
        optimalCurrentActive = false;
        buttons &= ~optimalCurrent;
   }
   else if (optimalCurrentActive){
        ccActive = false;
        buttons &= ~cc;
   }
}
