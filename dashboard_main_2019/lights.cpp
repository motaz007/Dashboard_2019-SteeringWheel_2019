/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * 
 */

#include "lights.h"

static bool swheelLedOn = false;


void turnOffStrip(Adafruit_NeoPixel& strip, const uint16_t& start, uint16_t end) {
    if (end == 0) {
        end = strip.numPixels();
    }

    for (uint16_t i = start; i < end; ++i) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 0)); //turn off leds one by one
    }

    strip.show();
    swheelLedOn = false;
}


/*-------------------------- SWHEEL FUNCTIONS -------------------------*/
void startUpSwheelLight(Adafruit_NeoPixel& shweelLights){
    shweelLights.setBrightness(100);
    shweelLights.begin();
    for (int i = shweelLights.numPixels(); i >= 0; --i) {
      shweelLights.setPixelColor(i, shweelLights.Color(255, 255, 255, 255));
     shweelLights.show();

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

/*-------------------------- DASH FUNCTIONS -------------------------*/
void drivingLightsFront(Adafruit_NeoPixel& frontLights) {
    for (uint16_t i = 0; i < frontLights.numPixels(); ++i) {
        frontLights.setPixelColor(i, COLOR_FRONTLIGHTS);
    }

    frontLights.show();
}

void drivingLightsBack(Adafruit_NeoPixel& backLights, int ledBrightness) {
    //backLights.setBrightness(ledBrihtness);
    for (uint16_t i = 0; i < backLights.numPixels(); ++i) {
        backLights.setPixelColor(i, COLOR_BACKLIGHTS);
    }

    backLights.show();
}

void setStripBrightness(Adafruit_NeoPixel& strip, int ledBrightness) {
    //strip.setBrightness(ledBrihtness);
//    backLights.show();
}
