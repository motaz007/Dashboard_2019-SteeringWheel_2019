#include "helpers.h"

int throttleRaw;
int throttle;
int regenRaw;
int regen;


int readThrottle(int PIN_THROTTLE, volatile uint8_t buttons[], bool ccActive, bool optimalCurrentActive) {
    static const int THROTTLE_RAW_LOW  = 405;
    static const int THROTTLE_RAW_HIGH = 525;
    static const int THROTTLE_LOW  = 0;
    static const int THROTTLE_HIGH = 255;
    static const int THROTTLE_THRESHOLD = 20;
    
    throttle = map(throttleRaw, THROTTLE_RAW_LOW, THROTTLE_RAW_HIGH, THROTTLE_LOW, THROTTLE_HIGH);
    
    if (!ccActive && !optimalCurrentActive) {
        // only read new throttle command if cc is inactive (i.e. off)
        throttleRaw = analogRead(PIN_THROTTLE);
    }
    else if (ccActive && !optimalCurrentActive) {
        // is active (i.e. on)
        Serial.print(" CC ");
    }
    else if (!ccActive && optimalCurrentActive) {
        throttle = OPTIMAL_CURRENT_VAL;
        Serial.print(" OPTIMAL CURRENT ");

        // to get cc to work corretly
        // +1 at end because map returned 29...
        throttleRaw = map(throttle, THROTTLE_LOW, THROTTLE_HIGH, THROTTLE_RAW_LOW, THROTTLE_RAW_HIGH) + 1;
    }
    
    if (throttle <= THROTTLE_THRESHOLD) {
        throttle = THROTTLE_LOW;
    }
    else if (throttle >= THROTTLE_HIGH - THROTTLE_THRESHOLD) {
        throttle = THROTTLE_HIGH;
    }

    Serial.print(" T: "); Serial.print(throttleRaw); Serial.print("-"); Serial.print(throttle); Serial.println(" ");    
    return throttle;
}

int readRegen(int PIN_REGEN, volatile uint8_t *buttons, bool ccActive, bool optimalBrakeActive) {
    static const int REGEN_RAW_LOW  = 444;
    static const int REGEN_RAW_HIGH = 542;
    static const int REGEN_LOW  = 0;
    static const int REGEN_HIGH = 255;
    static const int REGEN_THRESHOLD = 3;

    regenRaw = analogRead(PIN_REGEN);
    regen = map(regenRaw, REGEN_RAW_LOW, REGEN_RAW_HIGH, REGEN_LOW, REGEN_HIGH);

    
    if (regen <= REGEN_THRESHOLD) {
        regen = REGEN_LOW;
    }
    else if (regen >= REGEN_HIGH - REGEN_THRESHOLD) {
        regen = REGEN_HIGH;
    }

    if (optimalBrakeActive) {
        regen = OPTIMAL_BRAKE_VAL;
        Serial.print(" OPTIMAL BRAKE ");
    }

    Serial.print(" R: "); Serial.print(regenRaw); Serial.print("-"); Serial.print(regen); Serial.print(" ");    
    return regen;
}

void clockSpeed2Mhz(bool debug){ //Serial will not be available at this clockspeed
  if(debug == false){
    SIM_CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(7) | // Core/system clock 
                  SIM_CLKDIV1_OUTDIV2(7) | // peripheral clock 
                  SIM_CLKDIV1_OUTDIV3(7) | // CANbus Clock
                  SIM_CLKDIV1_OUTDIV4(7)); // flash clock   
  }
}

void clockSpeed120Mhz(bool debug){
  if(debug == false){
    SIM_CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0) | // Core/system clock
                  SIM_CLKDIV1_OUTDIV2(0) | // peripheral clock 
                  SIM_CLKDIV1_OUTDIV3(7) | // CANbus Clock
                  SIM_CLKDIV1_OUTDIV4(7)); // flash clock   
  }
}
