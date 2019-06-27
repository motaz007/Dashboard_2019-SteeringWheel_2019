/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * 
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

// SCREEN 1
#define S1_SCK  13
#define S1_MOSI 11
#define S1_CS   10

// SCREEN 2
#define S2_SCK  32
#define S2_MOSI 21
#define S2_CS   31

#include <Adafruit_SharpMem.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>

/* SCREEN INCLUDES */
//include bitm
#include "fonts/FreeMono9pt7b.h"
#include "fonts/FreeMono12pt7b.h"
#include "fonts/FreeMono18pt7b.h"
#include "fonts/FreeMono24pt7b.h"

/* SCREEN PARAMETERS */
#define WIDTH  320
#define HEIGHT 240
#define BLACK 0
#define WHITE 1
Adafruit_SharpMem screen1(S1_SCK, S1_MOSI, S1_CS, WIDTH, HEIGHT);
Adafruit_SharpMem screen2(S2_SCK, S2_MOSI, S2_CS, WIDTH, HEIGHT);
enum ORIENTATION { UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 };
// UP is where the flap is on the screen case

/* FUNCTION PROTOTYPES */
// Clears specified screen
void clearScreen(Adafruit_SharpMem& screen);

// Draws the background bitmap defined in a .h file.
// The .h file must be exported from GIMP as .xbm and then renamed to .h. 
// `right` is to select which background to use.
//void drawBackground(Adafruit_SharpMem& screen, bool right=true);

// Draws a string at (x, y).
// The size of the text will be `size` times the font size specified
// by the font .h file. 
// `x` and `y` are the bottom left corner of the text string,
// so remember to take that into account when inputting the coords.
void drawString(Adafruit_SharpMem& screen, const char * str, int x, int y, int size=1);

void initScreen1();
void initScreen2();

#endif
