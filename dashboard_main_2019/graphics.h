/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * Library with code for the two SharpMem screens
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

//#include ".\\lib/Adafruit_SharpMem_edited/Adafruit_SharpMem_edited.h"
#include <Adafruit_SharpMem_edited_test>
#include <Adafruit_GFX.h>
#include <gfxfont.h>

/* SCREEN INCLUDES */
//Bitmaps for backgrounds:
#include "left.h"
#include "right.h"

//Fonts:
#include "fonts/FreeMono9pt7b.h"
#include "fonts/FreeMono12pt7b.h"
#include "fonts/FreeMono18pt7b.h"
#include "fonts/FreeMono24pt7b.h"

// RIGHT SCREEN PINS
#define SR_SCK  13
#define SR_MOSI 11
#define SR_CS   10

// LEFT SCREEN PINS
#define SL_SCK  32
#define SL_MOSI 21
#define SL_CS   31

/* SCREEN PARAMETERS */
#define WIDTH  320
#define HEIGHT 240
#define BLACK 0
#define WHITE 1

/*-------------------------- FUNCTIONS --------------------------*/

/*class screens : public Adafruit_sharpMem {
  
}*/


void clearScreen(Adafruit_SharpMem& screen); 
void drawBackground(Adafruit_SharpMem& screen, bool right=true); // Draws the background bitmap defined in a .h file.
void drawString(Adafruit_SharpMem& screen, const char * str, int x, int y, int size=1);

//INIT FUNCTIONS

//Test Functions//
void testfillrect(Adafruit_SharpMem disp);
void testdrawrect(Adafruit_SharpMem disp);


/*
void initScreen1(); //ta inn screen
void initScreen2(); //---||---
void initScreensContent(); //---||---

// DRAW FUNCTIONS FOR RIGHT SCREEN
void drawSpeed(Adafruit_SharpMem& screen, const float& motor1speed, const float& motor2speed); //ta inn can så lage funksjon som henter ut en bestemt posisjon i can.h?
void drawLapTime(Adafruit_SharpMem& screen, const int& lapTimeSeconds);
void drawCurrentValue(Adafruit_SharpMem& screen, const double& motor1current, const double& motor2current); //ta inn can så lage funksjon som henter ut en bestemt posisjon i can.h?
void getStateString(const int& state, char* str); //PLACE HERE?
void drawMotor1State(Adafruit_SharpMem& screen, const int& motor1); // motor states:OFF, ACCCEL, BRAKE, IDLE, ERROR OR ENGAGE
void drawMotor2State(Adafruit_SharpMem& screen, const int& motor2);// hente dette direkte fra txMsg?
void drawCC(Adafruit_SharpMem& screen, const bool& active, const int& throttle); //samme som over?


void drawClutch(Adafruit_SharpMem& screen, const int& motor1clutch, const int& motor2clutch); //can-melding som parameter?

// DRAW FUNCTINOS FOR LEFT SCREEN
void drawTimeLeft(Adafruit_SharpMem& screen, const int& timeLeft);
void drawLapCount(Adafruit_SharpMem& screen, volatile const uint8_t& count, const uint8_t& max=10);
void drawBestAndAvgLapTime(Adafruit_SharpMem& screen, volatile const int* lapTimes, const uint8_t& size);
void drawVoltageValue(Adafruit_SharpMem& screen, const double& voltageVal);
void drawTemperature(Adafruit_SharpMem& screen, const int& motor1temp, const int& motor2temp);
void drawTotalEnegry(Adafruit_SharpMem& screen, const float& motor1totalEnergy, const float& motor2totalEnergy);*/

#endif
