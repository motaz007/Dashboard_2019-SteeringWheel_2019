/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * early version of lights file, lacking comments (see therefore the main steering wheel code
 */

#include "lights.h"

static bool swheelLedOn = false;

void turnOffStrip(Adafruit_NeoPixel& strip, const uint16_t& start, uint16_t end) {
    if (end == 0) {
        end = strip.numPixels();
    }

    for (uint16_t i = start; i < end; ++i) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }

    strip.show();
    swheelLedOn = false;
}

void startUpSwheelLight(Adafruit_NeoPixel& shweelLights){
    shweelLights.setBrightness(40);
    shweelLights.begin();
    for (int i = shweelLights.numPixels(); i >= 0; --i) {
      shweelLights.setPixelColor(i, COLOR_ERROR);
      shweelLights.show();
      delay(100);
    }
     delay(1000);
     swheelLedOn = true;
}

void swheelError(Adafruit_NeoPixel& shweelLights){
  if(swheelLedOn){
    for (int i = 0; i < shweelLights.numPixels(); ++i) {
      shweelLights.setPixelColor(i, COLOR_ERROR);
      
    }
    shweelLights.show();
  }
}

void swheelOK(Adafruit_NeoPixel& shweelLights) {
  if(swheelLedOn){
    for (int i = 0; i < shweelLights.numPixels(); ++i) {
      shweelLights.setPixelColor(i, COLOR_SWHEELLIGHTS);
      
    }
    shweelLights.show();
  }
}
