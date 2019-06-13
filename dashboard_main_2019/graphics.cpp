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
    char Lap[] = "Lap";

    char sector[32] = "Sector";
    
  if(!right) {
    screen.setTextColor(BLACK);
    screen.setFont(&Open_Sans_Bold_10);
    drawString(screen, Lap, 170, 90, 1);

    
    screen.setFont(&Open_Sans_Regular_10);
    screen.setTextColor(WHITE);
    drawString(screen, "Lap ", 153,185, 1);
    screen.setFont(&Open_Sans_Bold_10);
    drawString(screen, "time", 180, 185, 1);

    
    screen.setFont(&Open_Sans_Regular_10);
    drawString(screen, "Sector",  50, 270, 1);
    screen.write('r'); //må fikses så man kan ha lengre strenger


    drawString(screen, "Time",  150, 265, 1);
    screen.setFont(&Open_Sans_Bold_10);
    drawString(screen, "total",  180, 265, 1);
    
  } else if (right) {
      screen.setFont(&Open_Sans_Regular_12);
      screen.setTextColor(BLACK);
      drawString(screen, "km/h",  163, 80, 1);
      screen.setTextColor(WHITE);
      screen.setFont(&Open_Sans_Regular_10);
      drawString(screen, "Voltage",  170, 212, 1);
      drawString(screen, "ge  ",  200, 212, 1);
      
      drawString(screen, "Current",  170, 257, 1);
      drawString(screen, "nt  ",  200, 257, 1);

      drawString(screen, "Power",  177, 302, 1);

      screen.setFont(&Open_Sans_Bold_18);
      drawString(screen, "V   ", 200, 200, 1);
      drawString(screen, "A   ", 200, 245, 1);
      drawString(screen, "W   ", 195, 290, 1);
  }
    
}



//DRAW FUNCTIONS FOR LEFT SCREEN

void drawLapCount(Adafruit_SharpMem& screen, volatile const uint8_t& lapCount, const uint8_t& maxLaps) {

    char str[16];
    if (lapCount < 10) {
      sprintf(str, "%02u/%02u", lapCount, maxLaps); //formating string for numbers with one siffer
    } else {
      sprintf(str, "%02u/%02u", lapCount, maxLaps); //formating string with two
    }
   
    screen.fillRect(lapCountHorizontalMargin, lapCountVerticalMargin-50/1.2, lapCountHorizontal+40, 60, WHITE); //fiks variabler. dette er litt for random. sett opp skisse og tegn
    screen.setFont(&Open_Sans_Bold_60);
    screen.setTextColor(BLACK);
    drawString(screen, str, lapCountHorizontalMargin, lapCountVerticalMargin+10, 1);
}

void drawLapTime(Adafruit_SharpMem& screen, const int& lapTimeSeconds) {

    // format time
    int mins = lapTimeSeconds / 60;
    int secs = lapTimeSeconds % 60;


    char str[16] = {0};
    sprintf(str, "%02u:%02u", mins, secs);
    

    screen.fillRect(lapTimeHorizontalMargin, lapTimeVerticalMargin - M24/1.2, lapTimeHorizontal, M24, BLACK);
    screen.setFont(&Open_Sans_Bold_36);
    screen.setTextColor(WHITE);
    drawString(screen, str, 110, 170, 1);
}

void drawTime(Adafruit_SharpMem& screen, const int& timeSeconds) {

    // format time
    int mins = timeSeconds  / 60;
    int secs = timeSeconds  % 60;


    char str[16] = {0};
    sprintf(str, "%02u:%02u", mins, secs);
    

    screen.fillRect(timeHorizontalMargin, timeVerticalMargin - M18/1.2, timeHorizontal, M18, BLACK);
    screen.setFont(&Open_Sans_Bold_18);
    screen.setTextColor(WHITE);
    drawString(screen, str, 155, 250, 1);
}

void drawRectangle(Adafruit_SharpMem& screen) {
    screen.fillRect(44, 212, 45, 45, WHITE);
    screen.fillRect(46, 214, 41, 41, BLACK);

}

void drawSector(Adafruit_SharpMem& screen, int sector) {
    screen.fillRect(45, 210, 50, 50, BLACK);
    drawRectangle(screen);
    char str[8] = {0};
    sprintf(str, "%u", sector);
    screen.setFont(&Open_Sans_Bold_36);
    screen.setTextColor(WHITE);
    drawString(screen, str, 55, 249, 1);
}

//DRAW FUNCTIONS FOR RIGHT SCREEN


void drawSpeed(Adafruit_SharpMem& screen, const float& motor1speed, const float& motor2speed) {
    uint8_t speedThreshold = 10;
    float motorSpeed = 0;

    screen.setFont(&Open_Sans_Bold_60);
    screen.setTextColor(BLACK);
    screen.fillRect(40, 35, 115, 60, WHITE);
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

void drawGear(Adafruit_SharpMem& screen, const char gear) {
    screen.fillRect(6, 138, 120, 45, BLACK);
    
    screen.setFont(&Open_Sans_Bold_36);
    screen.setTextColor(WHITE);

    switch(gear) {
      case '1':
      drawCirkle(screen, 26, 157, 18);
      break;
      case 'N':
      drawCirkle(screen, 65, 158, 20);
      break;
      case '2':
      drawCirkle(screen, 105, 158, 18);
    }
    
    
    drawString(screen, "1   ", 15, 170, 1);
    drawString(screen, "N    ", 50, 170, 1);
    drawString(screen, "2   ", 95, 170, 1);
}

void drawCirkle(Adafruit_SharpMem& screen, const uint8_t x, const uint8_t y, const uint8_t r) {
   screen.fillCircle(x, y, r, WHITE);
   screen.fillCircle(x, y, r-2, BLACK);
}

void drawLightIcon(Adafruit_SharpMem& screen, bool on) {
    if(on) {
      screen.drawXBitmap(150, 150, lightIcon_bits, 21, 22, WHITE);
    } else {
      screen.fillRect(150, 150, 21, 22, BLACK);
    }
    
}
    

void drawVoltageValue(Adafruit_SharpMem& screen, const double& voltageVal) {

    char str[16];
    sprintf(str, "%-4.2f", voltageVal);

    screen.fillRect(140, 186, 55, 15, BLACK);
    screen.setFont(&Open_Sans_Bold_18);
    screen.setTextColor(WHITE);
    drawString(screen, str, 145, 200, 1);
}

void drawCurrentValue(Adafruit_SharpMem& screen, const double& motor1current, const double& motor2current) {
    double current = (motor1current + motor2current)/2;

    char str[16] = {0};
    sprintf(str, "%-4.2f", current);

    screen.setFont(&Open_Sans_Bold_18);
    screen.setTextColor(WHITE);
    screen.fillRect(140, 230, 55, 15, BLACK);
    drawString(screen, str, 145, 245, 1);
}

void drawPowerConsumption(Adafruit_SharpMem& screen, const double& motor1power, const double& motor2power) {
    double current = (motor1power + motor2power)/2;

    char str[16] = {0};
    sprintf(str, "%-4.2f", current);

    screen.setFont(&Open_Sans_Bold_18);
    screen.setTextColor(WHITE);
    screen.fillRect(140, 275, 55, 15, BLACK);
    drawString(screen, str, 145, 290, 1);
}

void drawRaceMode(Adafruit_SharpMem& screen, bool on) {
    screen.fillRect(5, 300, 20, 20, BLACK);
    if(on) {
      screen.setFont(&Open_Sans_Bold_10);
      screen.setTextColor(WHITE);
      drawString(screen, "R", 10, 310, 1);      
    }
}
