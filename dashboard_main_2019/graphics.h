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

//Fonts:
#include "fonts/FreeMono9pt7b.h"
#include "fonts/FreeMono12pt7b.h"
#include "fonts/FreeMono18pt7b.h"
#include "fonts/FreeMono24pt7b.h"

#include "fonts/FreeMonoBold9pt7b.h"
#include "fonts/FreeMonoBold12pt7b.h"
#include "fonts/FreeMonoBold18pt7b.h"
#include "fonts/FreeMonoBold24pt7b.h"

enum ORIENTATION { UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 };

/* SCREEN PARAMETERS */
#define WIDTH  320
#define HEIGHT 240
#define BLACK 0
#define WHITE 1

#define LEFTSCREEN false
#define RIGHTSCREEN true

/*----------- SCREEN VARIABLES ----------- */



const uint8_t lapCountVerticalMargin = 65;
const uint8_t lapCountHorizontalMargin = 30;
const uint8_t lapCountVertical = 60;
const uint8_t lapCountHorizontal = 140;


/*-------------------------- FUNCTIONS --------------------------*/

/*class screens : public Adafruit_sharpMem {
  
}*/


void drawBackground(Adafruit_SharpMem& screen, bool right=true); // Draws the background bitmap defined in a .h file.
void drawString(Adafruit_SharpMem& screen, const char * str, int x, int y, int size=1);

//INIT FUNCTIONS

void initScreen(Adafruit_SharpMem& screen, bool right);
void initText(Adafruit_SharpMem& screen, bool right);

//DRAW FUNCTIONS FOR LEFT SCREEN
void drawLapCount(Adafruit_SharpMem& screen, volatile const uint8_t& lapCount, const uint8_t& maxLaps);



#endif
