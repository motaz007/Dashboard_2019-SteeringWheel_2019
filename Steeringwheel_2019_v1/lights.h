#include <Adafruit_NeoPixel.h>

#ifndef LIGHTS_H_
#define LIGHTS_H_

/*-------------------------- VARIABLES -------------------------*/

//COLORS
// Colors are defined as uint32_t using the following formula
// white << 24 | red << 16 | green << 8 | blue   << 0
#define COLOR_BACKLIGHTS  16711680  // WRGB = 0,255,0,0
#define COLOR_FRONTLIGHTS 4278190080 // WRGB = 255,0,0,0
#define COLOR_BLINKLIGHTS 16737280  // WRGB = 0,255,80,0
#define COLOR_SWHEELLIGHTS 990975
#define COLOR_ERROR 16724736UL  // WRGB = 0,255,51,0

#define colorOrange 16724736UL  // WRGB = 0,255,51,0
#define colorSwheelLights 990975

//BACK
#define NUM_BACKLIGHTS  288  
#define BRIGHTNESS_BACK 100
#define BRIGHTNESS_BACK_BRAKE 100
#define BLINK_LEFT_START_BACKLIGHTS  0
#define BLINK_LEFT_END_BACKLIGHTS    38
#define BLINK_RIGHT_START_BACKLIGHTS 255
#define BLINK_RIGHT_END_BACKLIGHTS   217

//FRONT
#define NUM_FRONTLIGHTS  212  
#define BRIGHTNESS_FRONT 100
#define BLINK_LEFT_START_FRONTLIGHTS  212
#define BLINK_LEFT_END_FRONTLIGHTS    175
#define BLINK_RIGHT_START_FRONTLIGHTS 0
#define BLINK_RIGHT_END_FRONTLIGHTS   34

//SWHEEL
#define NUM_SWHEEL_LIGHTS 13
#define BRIGHTNESS_SWHEEL 40

void startUpSwheelLight(Adafruit_NeoPixel& sWheelLights);
void sWheelLight(Adafruit_NeoPixel& sWheelLights, bool deadManSwitch);
void turnOffStrip(Adafruit_NeoPixel& strip, const uint16_t& start, uint16_t end);
void rgbTest(Adafruit_NeoPixel& strip);


#endif
