/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * 
 */

#include "helpers.h"

void initSerial() {
    // to communicate with the computer
    Serial.begin(SERIAL_BAUDRATE);
}

void clockSpeedLow(bool debug){ //Serial will not be available at this clockspeed
  if(debug == false){
    SIM_CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(7) | // Core/system clock 
                  SIM_CLKDIV1_OUTDIV2(7) | // peripheral clock 
                  SIM_CLKDIV1_OUTDIV3(7) | // CANbus Clock
                  SIM_CLKDIV1_OUTDIV4(7)); // flash clock   
  }
}

void clockSpeedHigh(bool debug){
  if(debug == false){
    SIM_CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0) | // Core/system clock
                  SIM_CLKDIV1_OUTDIV2(0) | // peripheral clock 
                  SIM_CLKDIV1_OUTDIV3(7) | // CANbus Clock
                  SIM_CLKDIV1_OUTDIV4(7)); // flash clock    
  }
}
