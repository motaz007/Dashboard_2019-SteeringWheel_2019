/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * 
 */

#include <Adafruit_NeoPixel.h>

#ifndef LIGHTS_H_
#define LIGHTS_H_

// LIGHTS
#define PIN_BACKLIGHT   35
#define PIN_FRONTLIGHT   36

#define COLOR_ERROR 16724736UL  // WRGB = 0,255,51,0
#define COLOR_SWHEELLIGHTS 990975

void startUpSwheelLight(Adafruit_NeoPixel& shweelLights);
void swheelError(Adafruit_NeoPixel& shweelLights);
void swheelOK(Adafruit_NeoPixel& shweelLights);
void turnOffStrip(Adafruit_NeoPixel& strip, const uint16_t& start, uint16_t end);


#endif
