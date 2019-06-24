/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * Library with code for the two SharpMem screens
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "helpers.h"

#include <Adafruit_SharpMem_edited.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>

/* SCREEN INCLUDES */
//Bitmaps for backgrounds:
#include "left.h"
#include "right.h"

/* Bitmap for icons */
#include "lightIcon.h"

//Fonts:

//BOLD
#include "fonts/Open_Sans_Bold_60.h"
#include "fonts/Open_Sans_Bold_45.h"
#include "fonts/Open_Sans_Bold_36.h"
#include "fonts/Open_Sans_Bold_24.h"
#include "fonts/Open_Sans_Bold_18.h"
#include "fonts/Open_Sans_Bold_12.h"
#include "fonts/Open_Sans_Bold_10.h"


//REGULAR
#include "fonts/Open_Sans_Regular_60.h"
#include "fonts/Open_Sans_Regular_36.h"
#include "fonts/Open_Sans_Regular_24.h"
#include "fonts/Open_Sans_Regular_18.h"
#include "fonts/Open_Sans_Regular_12.h"
#include "fonts/Open_Sans_Regular_10.h"


enum ORIENTATION { UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 };

/* SCREEN PARAMETERS */
#define WIDTH  320
#define HEIGHT 240
#define BLACK 0
#define WHITE 1

#define LEFTSCREEN false
#define RIGHTSCREEN true

/*----------- SCREEN VARIABLES ----------- */

//FONT HEIGHTS
//this is used because rectangles starts top left and text buttom left
const uint8_t fontHeight60 = 48;
const uint8_t fontHeight45 = 33;
const uint8_t fontHeight36 = 27;
const uint8_t fontHeight18 = 15;
const uint8_t fontHeight12 = 10;                                                            //not used, therefore not tested. guess value
const uint8_t fontHeight10 = 7;
const uint8_t textVerticalMargin = 12;                                                      //space between lines

//VERITCAL MARGINS

const uint8_t speedThreshold = 10;                                                      //move to global? or argument?  
const uint8_t yMarginSpeedLapCount = 100;                                                    //margin to bottom line of lap count and speed

//Lap Count
const uint8_t lapCountVerticalMargin = 65; //to bottom left?
const uint8_t lapCountHorizontalMargin = 30;
const uint8_t lapCountVertical = 60;
const uint8_t lapCountHorizontal = 140;

//Lap Time
const uint8_t lapTimeVerticalMargin = 170; //to bottom left?
const uint8_t lapTimeHorizontalMargin = 90;
const uint8_t lapTimeVertical = 200;
const uint8_t lapTimeHorizontal = 140;

//Tme 
const uint8_t timeHorizontalMargin = 125;
const uint8_t timeVerticalMargin = 250;
const uint8_t timeHorizontal = 105;

/*-------------------------- FUNCTIONS --------------------------*/

/*class screens : public Adafruit_sharpMem {
  
}*/


void drawBackground(Adafruit_SharpMem& screen, bool right=true);                            // Draws the background bitmap defined in a .h file.
void drawString(Adafruit_SharpMem& screen, const char * str, int x, int y, int size=1);
void drawStringLong(Adafruit_SharpMem& screen, const char * str, int x, int y, int size=1); //denne trengs!!

//INIT FUNCTIONS

void initScreen(Adafruit_SharpMem& screen, bool right);
void initText(Adafruit_SharpMem& screen, bool right);

//DRAW FUNCTIONS FOR LEFT SCREEN
void drawLapCount(Adafruit_SharpMem& screen, volatile const uint8_t& lapCount, const uint8_t& maxLaps);
void drawLapTime(Adafruit_SharpMem& screen, const int& lapTimeSeconds);
void drawTime(Adafruit_SharpMem& screen, const int& timeSeconds);
void drawRectangle(Adafruit_SharpMem& screen);
void drawSector(Adafruit_SharpMem& screen, int sector);


//DRAW FUNCTIONS FOR RIGHT SCREEN
void drawSpeed(Adafruit_SharpMem& screen, const float& motor1speed, const float& motor2speed);
void drawGear(Adafruit_SharpMem& screen, const char gear);
void drawCirkle(Adafruit_SharpMem& screen, const uint8_t x, const uint8_t y, const uint8_t r);
void drawLightIcon(Adafruit_SharpMem& screen, bool on);
void drawVoltageValue(Adafruit_SharpMem& screen, const double& voltageVal);
void drawCurrentValue(Adafruit_SharpMem& screen, const double& motor1current, const double& motor2current);
void drawPowerConsumption(Adafruit_SharpMem& screen, const double& motor1power, const double& motor2power);
void drawRaceMode(Adafruit_SharpMem& screen, bool on);

#endif
