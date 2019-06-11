#include "graphics.h"

void drawBackground(Adafruit_SharpMem& screen, bool right) {
    int X = 0,
        Y = 0;

    if (right) {
        screen.drawXBitmap(X, Y, left_bits, WIDTH, HEIGHT, BLACK);
    }
    else {
        screen.drawXBitmap(X, Y, right_bits, WIDTH, HEIGHT, BLACK);
    }
}

void drawString(Adafruit_SharpMem& screen, const char * str, int x, int y, int size) {
    screen.setCursor(x, y);
    screen.setTextSize(size);

    for (unsigned int i = 0; i < sizeof(str)/sizeof(str[0]) + 1; ++i) {
        screen.write(str[i]);  // `.write` moves cursor for every character
    }
  
}

//INIT FUNCTIONS

void initScreen(Adafruit_SharpMem& screen, bool right) {
    screen.begin();
    screen.clearDisplay();
    drawBackground(screen, right);
    screen.setRotation(ORIENTATION::LEFT);
    screen.refresh();
}

void initText(Adafruit_SharpMem& screen, bool right) {
    char str1[] = "Laps";
    char str2[] = "taken";
  
  if(!right) {
    screen.setTextColor(BLACK);
    screen.setFont(&FreeMonoBold9pt7b);
    drawString(screen, str1, 60, 90, 1);

    screen.setFont(&FreeMono9pt7b);
    drawString(screen, str2, 110, 90, 1);

  }
    
}



//DRAW FUNCTIONS FOR LEFT SCREEN

void drawLapCount(Adafruit_SharpMem& screen, volatile const uint8_t& lapCount, const uint8_t& maxLaps) {

    char str[16];
    if (lapCount < 10) {
      sprintf(str, " %u/%u", lapCount, maxLaps); //formating string for numbers with one siffer
    } else {
      sprintf(str, "%u/%u", lapCount, maxLaps); //formating string with two
    }
   
    screen.fillRect(lapCountHorizontalMargin, lapCountVerticalMargin-lapCountVertical/1.16, lapCountHorizontal, lapCountVertical, WHITE);
    screen.setFont(&FreeMonoBold12pt7b);
    drawString(screen, str, lapCountHorizontalMargin, lapCountVerticalMargin, 2);
}
