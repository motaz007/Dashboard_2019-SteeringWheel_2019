#include <Adafruit_NeoPixel.h>

#ifndef LIGHTS_H_
#define LIGHTS_H_

#define COLOR_ERROR 16724736UL  // WRGB = 0,255,51,0
#define COLOR_SWHEELLIGHTS 990975

void startUpSwheelLight(Adafruit_NeoPixel& sWheelLights);
void sWheelError(Adafruit_NeoPixel& sWheelLights);
void sWheelOK(Adafruit_NeoPixel& sWheelLights);
void turnOffStrip(Adafruit_NeoPixel& strip, const uint16_t& start, uint16_t end);
void rgbTest(Adafruit_NeoPixel& strip);


#endif
