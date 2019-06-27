// MUST BE COMPILED AT 120 MHZ!!!


#include "can.h"
#include "lights.h"
#include "helpers.h"

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
bool deadManSwitch = false;
bool brakeOrAccPressed = false;

int CcCounter = 1;

static volatile uint8_t buttons, buttons2;

uint8_t dashValues[8];

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
  
  initValuesSWheel(txMsg);
  buttons = 0;
  ccActive = false;
  optimalCurrentActive = false;
  optimalBrakeActive = false;
  clockSpeed120Mhz(debug);
  startUpSwheelLight(swheelLights);
  delay(500);

  initPins();
  clockSpeed2Mhz(debug);
}

void loop(){ 
    
    txMsg.buf[0] = buttons2;  
    txMsg.buf[1] = buttons;
    txMsg.buf[2] = readRegen(PIN_REGEN, &buttons , ccActive, optimalBrakeActive);
    txMsg.buf[3] = readThrottle(PIN_THROTTLE, &buttons, ccActive, optimalCurrentActive);
    brakeEnabled();
    readCan(rxMsg);
    writeCan(txMsg);
    printCanToSerial(txMsg, debug);
    delay(50);
   /* Serial.print(" ccActive: ");
    Serial.println(ccActive);
    Serial.print(" optimalBrakeActive: ");
    Serial.println(optimalBrakeActive);
    Serial.print(" optimalCurrentActive: ");
    Serial.println(optimalCurrentActive);
    */
    dashCAN(txMsg);


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
          Serial.println("  LAP UNPRESSED  ");
      }
      if (state == LOW && interrupt_time - last_interrupt_time > 200) {
          buttons |= lap;
          Serial.println("  LAP PRESSED  ");
      }
     }
    last_interrupt_time = interrupt_time;
}

void deadmanSwitchChanged_ISR() {
    int state = digitalRead(PIN_DEADMAN_SWITCH);

    if (state == HIGH) {  // RISING -- is unpressed
        deadManSwitch = false;
        buttons &= ~deadmanSwitch;
        clockSpeed120Mhz(debug); // Rising clockspeed to be able to change lights
        sWheelLight(swheelLights, deadManSwitch);
        clockSpeed2Mhz(debug);
        Serial.println("  DMS UNPRESSED!  ");
    }
    else {  
        deadManSwitch = true;
        buttons |= deadmanSwitch;
        clockSpeed120Mhz(debug); // Raising clockspeed to be able to change lights
        sWheelLight(swheelLights, deadManSwitch);
        //clockSpeed2Mhz(debug);
        Serial.println("  DMS PRESSED!  ");
    }
}

void leftBlinkerChanged_ISR() {
    int state = digitalRead(PIN_BLINKER_L);
    int leftBlinkerPressed;
    
    if (state == HIGH && leftBlinkerPressed == LOW) {  // RISING -- is unpressed
        leftBlinkerPressed = false;
        buttons &= ~leftBlink;
        Serial.println("  BLINK L UNPRESSED  ");
    }
    else {
        leftBlinkerPressed = true;
        buttons |= leftBlink;
        Serial.println("  BLINK L PRESSED  ");
    }
}

void rightBlinkerChanged_ISR() {
    int state = digitalRead(PIN_BLINKER_R);
    int rightBlinkerPressed;

    if (state == HIGH && rightBlinkerPressed == LOW) {  // RISING -- is unpressed
        rightBlinkerPressed = false;
        buttons &= ~rightBlink;
        Serial.println("  BLINK R UNPRESSED  ");
    }
    else {
        rightBlinkerPressed = true;
        buttons |= rightBlink;
        Serial.println("  BLINK R PRESSED  ");
    }
}

void hornChanged_ISR() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 10ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 10)
  {
    int state = digitalRead(PIN_HORN);

    if(state == HIGH){
      buttons &= ~horn;
      Serial.println(" HORN UNPRESSED ");
      
    }else{
      buttons |= horn;
      Serial.println(" HORN PRESSED ");
    }
  }
  last_interrupt_time = interrupt_time;
}

void ccButtonPressed_ISR() {  // CC = Counter for Optimal Current
     static unsigned long last_interrupt_time = 0;
     unsigned long interrupt_time = millis();
     // If interrupts come faster than 200ms, assume it's a bounce and ignore
      if (interrupt_time - last_interrupt_time > 200)
     {
      int state = digitalRead(PIN_LAP_BUTTON);
      
      if (state == HIGH) {  // RISING -- is unpressed
          //buttons &= ~cc;
          ccActive = true;
          txMsg.buf[7] = CcCounter;
          CcCounter++;
          if(CcCounter > CC_COUNTER_MAX){
          CcCounter = 1;
        }
          Serial.println("  CC UNPRESSED  ");
      }else{
          //buttons |= cc;
          ccActive = false;
          Serial.println("  CC PRESSED  ");
      }
     }
    last_interrupt_time = interrupt_time;
}

void optimalCurrentButtonChanged_ISR() {
     static unsigned long last_interrupt_time = 0;
     unsigned long interrupt_time = millis();
     // If interrupts come faster than 10ms, assume it's a bounce and ignore
     if (interrupt_time - last_interrupt_time > 200)
     {
        if (optimalCurrentActive) {  // RISING -- is unpressed
            buttons &= ~optimalCurrent;
            optimalCurrentActive = false;
            Serial.println("  OPT-CURR OFF  ");
        }else if(!brakeOrAccPressed){
            optimalBrakeActive = false;
            buttons &= ~optimalBrake;
            buttons |= optimalCurrent;
            optimalCurrentActive = true;
            Serial.println("  OPT-CURR ON  ");
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
      int state = digitalRead(PIN_OPTIMAL_BRAKE);
        if (state == LOW &&  optimalBrakeActive) {  // RISING -- is unpressed
            buttons &= ~optimalBrake;
            optimalBrakeActive = false;
            Serial.println("  OPT-BRAKE OFF  ");
        }
        else if(!brakeOrAccPressed){
          optimalCurrentActive = false;
          buttons &= ~optimalCurrent;
          buttons |= optimalBrake;
          optimalBrakeActive = true;
          Serial.println("  OPT-BRAKE ON  ");
        }
     }
   last_interrupt_time = interrupt_time;
}

void blankButtonChanged_ISR() {
  int state = digitalRead(PIN_BLANK);

  if (state == HIGH) {  // RISING -- is unpressed
      buttons2 &= ~blank;
      Serial.println(" BLANK UNPRESSED! ");
  }
  else {
      buttons2 |= blank;
      Serial.println(" BLANK PRESSED ");
  }
}

void brakeEnabled(){
  if (txMsg.buf[2] != 0 || txMsg.buf[3] != 0 || (dashValues[0] &= (1<<0))) { // brake or acceleration pressed
        brakeOrAccPressed = true;
        optimalCurrentActive = false; // turn off optimalCurrent
        buttons &= ~optimalCurrent; 
        optimalBrakeActive = false; // turn off optimalBrake
        buttons &= ~optimalBrake;
    }else{
        brakeOrAccPressed = false;
    }
   if (ccActive){
        optimalCurrentActive = false;
        buttons &= ~optimalCurrent;
        ccActive = false;
   }
}

void dashCAN(CAN_message_t& rxMsg){
  if(rxMsg.id == dashID){
    for(int i = 0; i < rxMsg.len; i++){
      dashValues[i] = rxMsg.buf[i];
    }
 }
}
