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
#define COLOR_BACKLIGHTS  16711680  //16719360  // WRGB = 0,255,51,0
#define COLOR_FRONTLIGHTS 9895830 // WRGB = 0,150,255,150
#define COLOR_BLINKLIGHTS 16737280  // WRGB = 0,255,80,0
#define COLOR_SWHEELLIGHTS 990975
#define COLOR_ERROR 16724736UL  // WRGB = 0,255,51,0

#define colorOrange 16724736UL  // WRGB = 0,255,51,0
#define colorSwheelLights 990975

//BACK
#define NUM_BACKLIGHTS  57  // 28 r + 29 l
#define BRIGHTNESS_BACK 40
#define BRIGHTNESS_BACK_BRAKE 100
#define BLINK_LEFT_START_BACKLIGHTS  28
#define BLINK_LEFT_END_BACKLIGHTS    57
#define BLINK_RIGHT_START_BACKLIGHTS 0
#define BLINK_RIGHT_END_BACKLIGHTS   28

//FRONT
#define NUM_FRONTLIGHTS  152  // 76 r + 76 l
#define BRIGHTNESS_FRONT 60
#define BLINK_LEFT_START_FRONTLIGHTS  76
#define BLINK_LEFT_END_FRONTLIGHTS    152
#define BLINK_RIGHT_START_FRONTLIGHTS 0
#define BLINK_RIGHT_END_FRONTLIGHTS   76

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
void blinkLights(Adafruit_NeoPixel& strip, const uint16_t& start=0, uint16_t end=0); // Enable blinkers in the back. Should blink once per function call. 
void drivingLightsFront(Adafruit_NeoPixel& frontLights);  // Enable constant driving lights in the front
void drivingLightsBack(Adafruit_NeoPixel& backLights, int ledBrightness); // Enable constant driving lights in the back
void setStripBrightness(Adafruit_NeoPixel& strip, int ledBrightness);
void hazzardLights(Adafruit_NeoPixel& lights);
void lightShow(Adafruit_NeoPixel& lights);
void initDashLights();

#endif
