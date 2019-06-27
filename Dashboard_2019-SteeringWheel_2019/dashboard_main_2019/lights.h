/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * 
 */

#include <Adafruit_NeoPixel.h>

#ifndef LIGHTS_H_
#define LIGHTS_H_

/*-------------------------- VARIABLES -------------------------*/

//COLORS
// Colors are defined as uint32_t using the following formula
// white << 24 | red << 16 | green << 8 | blue   << 0
#define COLOR_BACKLIGHTS  16711680  // WRGB = 0,255,0,0
#define COLOR_FRONTLIGHTS 4278190080 // WRGB = 255,0,0,0
//#define COLOR_FRONTLIGHTS 16724736UL // WRGB = 255,0,0,0
#define COLOR_BLINKLIGHTS 16737280  // WRGB = 0,255,80,0
#define COLOR_SWHEELLIGHTS 990975   
#define COLOR_ERROR 16724736UL  // WRGB = 0,255,51,0
#define COLOR_OFF 0


//BACK
#define NUM_BACKLIGHTS  288  
#define BRIGHTNESS_BACK 40
#define BRIGHTNESS_BACK_BRAKE 100
#define NUM_BLINK 38
#define BLINK_LEFT_START_BACKLIGHTS  0
#define BLINK_LEFT_END_BACKLIGHTS    38
#define BLINK_RIGHT_START_BACKLIGHTS 255
#define BLINK_RIGHT_END_BACKLIGHTS   217
#define RACE_LEFT_START_BACKLIGHTS   39
#define RACE_LEFT_END_BACKLIGHTS     77
#define RACE_RIGHT_START_BACKLIGHTS  178
#define RACE_RIGHT_END_BACKLIGHTS    254

//FRONT
#define NUM_FRONTLIGHTS  212  
#define BRIGHTNESS_FRONT 60
#define BLINK_LEFT_START_FRONTLIGHTS  212
#define BLINK_LEFT_END_FRONTLIGHTS    178
#define BLINK_RIGHT_START_FRONTLIGHTS 0
#define BLINK_RIGHT_END_FRONTLIGHTS   30
#define RACE_START_FRONTLIGHTS 95   
#define RACE_END_FRONTLIGHTS 116    

//SWHEEL
#define NUM_SWHEEL_LIGHTS 13
#define BRIGHTNESS_SWHEEL 40

/*-------------------------- FUNCTIONS -------------------------*/

void turnOffStrip(Adafruit_NeoPixel& strip, const uint16_t& start, uint16_t end);

//SWHEEL
void startUpSwheelLight(Adafruit_NeoPixel& sWheelLights);
void swheelError(Adafruit_NeoPixel& sWheelLights);
void swheelOK(Adafruit_NeoPixel& sWheelLights);

//DASH
void initLights(Adafruit_NeoPixel& frontLights, Adafruit_NeoPixel& backLights);
void showLights(Adafruit_NeoPixel& frontLights, Adafruit_NeoPixel& backLights);
void blinkLights(Adafruit_NeoPixel& frontLights, Adafruit_NeoPixel& backLights, bool left, bool raceModeON); // Enable blinkers in the back
void brakeLights(Adafruit_NeoPixel& backLights, bool brakeON);
bool hazardLights(Adafruit_NeoPixel& frontLights, Adafruit_NeoPixel& backLights, bool hazardLightsON, bool raceModeON);
void raceLights(Adafruit_NeoPixel& frontLights, Adafruit_NeoPixel& backLights);

#endif
