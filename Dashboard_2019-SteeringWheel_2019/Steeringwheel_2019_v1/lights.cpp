#include "lights.h"

static bool sWheelLedOn = false;

void turnOffStrip(Adafruit_NeoPixel& strip, const uint16_t& start, uint16_t end) {
    if (end == 0) {
        end = strip.numPixels();
    }

    for (uint16_t i = start; i < end; ++i) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }

    strip.show();
    sWheelLedOn = false;
}

void startUpSwheelLight(Adafruit_NeoPixel& sWheelLights){
    sWheelLights.setBrightness(40);
    sWheelLights.begin();
    for (int i = sWheelLights.numPixels(); i >= 0; --i) {
      sWheelLights.setPixelColor(i, COLOR_ERROR);
      sWheelLights.show();
      delay(100);
    }
     delay(1000);
     sWheelLedOn = true;
}

void sWheelLight(Adafruit_NeoPixel& sWheelLights, bool deadManSwitch){
  if(sWheelLedOn){
    if(deadManSwitch){
      for (int i = 0; i < sWheelLights.numPixels(); ++i) {
      sWheelLights.setPixelColor(i, sWheelLights.Color(0,0,0,255));
      }
      sWheelLights.show();
      }else{
    for (int i = 0; i < sWheelLights.numPixels(); ++i) {
      sWheelLights.setPixelColor(i, COLOR_ERROR);
      
    }
    sWheelLights.show();
  }
  }
}

double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float counter = 0;

void rgbTest(Adafruit_NeoPixel& strip){
  strip.setBrightness(100);
  const float pi = 3.14159;
  
  for (int i = strip.numPixels(); i >= 0; --i) {
    float redLevel = 1000*sin(counter/1000);
    float greenLevel = 1000*sin((counter/1000)+pi*2/3);
    float blueLevel = 1000*sin((counter/1000)+pi*4/3);
    
    float redOut = mapf(redLevel, -1000, 1000, 0, 255);
    float greenOut = mapf(greenLevel, -1000, 1000, 0, 255);
    float blueOut = mapf(blueLevel, -1000, 1000, 0, 255);
  
    uint32_t color = strip.Color(redOut, greenOut, blueOut, 0);

    counter += 1;
    
    strip.setPixelColor(i, color);
    strip.show();
    delay(50);
  }
}
