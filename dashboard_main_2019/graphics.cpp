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
    char Laps[] = "Laps";
    char taken[] = "taken";
    
    char Time[] = "Time";
    char used[] = "used";
    char thiis[] = "this";
    char lap[] = "lap ";
  if(!right) {
    screen.setTextColor(BLACK);
    screen.setFont(&FreeMonoBold9pt7b);
    drawString(screen, Laps, 60, 90, 1);

    drawString(screen, taken, 110, 90, 1);
    
    screen.setFont(&FreeMono9pt7b);
    screen.setTextColor(WHITE);
    drawString(screen, Time, 43, 190, 1);
    drawString(screen, used, 93, 190, 1);
    drawString(screen, thiis, 143,190, 1);
    drawString(screen, lap, 193, 190, 1);
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
   
    screen.fillRect(lapCountHorizontalMargin, lapCountVerticalMargin-M24/1.2, lapCountHorizontal, M24, WHITE); //fiks variabler. dette er litt for random. sett opp skisse og tegn
    screen.setFont(&FreeMonoBold24pt7b);
    screen.setTextColor(BLACK);
    drawString(screen, str, lapCountHorizontalMargin, lapCountVerticalMargin, 1);
}

void drawLapTime(Adafruit_SharpMem& screen, const int& lapTimeSeconds) {

    // format time
    int mins = lapTimeSeconds / 60;
    int secs = lapTimeSeconds % 60;


    char str[16] = {0};
    sprintf(str, "%02u:%02u", mins, secs);
    

    screen.fillRect(lapTimeHorizontalMargin, lapTimeVerticalMargin - M24/1.2, lapTimeHorizontal, M24, BLACK);
    screen.setFont(&FreeMonoBold24pt7b);
    screen.setTextColor(WHITE);
    drawString(screen, str, 90, 170, 1);
}

void drawRectangle(Adafruit_SharpMem& screen) {
    screen.fillRect(100, 200, 50, 50, WHITE);
    screen.fillRect(102, 202, 46, 46, BLACK);

}

//DRAW FUNCTIONS FOR RIGHT SCREEN

void drawCirkle(Adafruit_SharpMem& screen) {
   screen.fillCircle(100, 270, 10, WHITE);
   screen.fillCircle(100, 270, 8, BLACK);
}
