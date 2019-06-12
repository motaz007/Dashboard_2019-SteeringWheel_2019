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

void drawStringLong(Adafruit_SharpMem& screen, const char * str, int x, int y, int size) {
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

    char sector[32] = "Sector";
    
  if(!right) {
    screen.setTextColor(BLACK);
    screen.setFont(&FreeMonoBold9pt7b);
    drawString(screen, Laps, 60, 90, 1);

    drawString(screen, taken, 110, 90, 1);
    
    screen.setFont(&FreeMono9pt7b);
    screen.setTextColor(WHITE);
    drawString(screen, "Time", 43, 190, 1);
    drawString(screen, "used", 93, 190, 1);
    screen.setFont(&FreeMonoBold9pt7b);
    drawString(screen, "this", 143,190, 1);
    drawString(screen, "lap ", 193, 190, 1);

    
    screen.setFont(&FreeMono9pt7b);
    drawString(screen, "Sector",  38, 275, 1);
    screen.write('r'); //må fikses så man kan ha lengre strenger


    drawString(screen, "Time",  130, 275, 1);
    screen.setFont(&FreeMonoBold9pt7b);
    drawString(screen, "used",  180, 275, 1);
    
  } else if (right) {
      screen.setFont(&FreeMonoBold9pt7b);
      screen.setTextColor(BLACK);
      drawString(screen, "km/h",  160, 80, 1);
    
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

void drawTime(Adafruit_SharpMem& screen, const int& timeSeconds) {

    // format time
    int mins = timeSeconds  / 60;
    int secs = timeSeconds  % 60;


    char str[16] = {0};
    sprintf(str, "%02u:%02u", mins, secs);
    

    screen.fillRect(timeHorizontalMargin, timeVerticalMargin - M18/1.2, timeHorizontal, M18, BLACK);
    screen.setFont(&FreeMonoBold18pt7b);
    screen.setTextColor(WHITE);
    drawString(screen, str, 125, 250, 1);
}

void drawRectangle(Adafruit_SharpMem& screen) {
    screen.fillRect(45, 210, 50, 50, WHITE);
    screen.fillRect(47, 212, 46, 46, BLACK);

}

void drawSector(Adafruit_SharpMem& screen, int sector) {
    screen.fillRect(45, 210, 50, 50, BLACK);
    drawRectangle(screen);
    char str[8] = {0};
    sprintf(str, "%u", sector);
    screen.setFont(&FreeMonoBold24pt7b);
    screen.setTextColor(WHITE);
    drawString(screen, str, 55, 249, 1);
}

//DRAW FUNCTIONS FOR RIGHT SCREEN


void drawSpeed(Adafruit_SharpMem& screen, const float& motor1speed, const float& motor2speed) {
    uint8_t speedThreshold = 10;
    float motorSpeed = 0;

    screen.setFont(&FreeMonoBold24pt7b);
    screen.setTextColor(BLACK);
    screen.fillRect(40, 45, 120, 40, WHITE);
    if (abs(motor1speed-motor2speed) > speedThreshold) {
      drawString(screen, "err.", 40, 80, 1);
    } else {
      motorSpeed = (motor1speed + motor2speed)/2;
      char str[16] = {0};
      if (motorSpeed < 10) {
              sprintf(str, "%u%0.1f", motorSpeed, motorSpeed);
              drawString(screen, str, 40, 80, 1);
      } else {
              sprintf(str, "%0.1f", motorSpeed);
              drawString(screen, str, 40, 80, 1);
      }
    }  
}


void drawCirkle(Adafruit_SharpMem& screen) {
   screen.fillCircle(100, 270, 10, WHITE);
   screen.fillCircle(100, 270, 8, BLACK);
}
