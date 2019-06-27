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
    initText(screen, right);
    initScreenContent(screen, right);
    
    screen.refresh();
}

void initScreenContent(Adafruit_SharpMem& screen, bool right)
{
 if(right)
 {
   drawSpeed(screen, 0, 0);                                                                          //always starting up at a stand still
   drawGear(screen, 'N');                                                                            //always starting up in neutral
   drawLightIcon(screen, true);                                                                      //always starting up with lights
   drawVoltageValue(screen, 0, 0);                                                                   //init everything as zero
   drawCurrentValue(screen, 0, 0);
   drawPowerConsumption(screen, 0, 0);                                                                   
   drawRaceMode(screen, true);                                                                       //always starting up in race mode
 } else if (!right) 
 {
   drawLapCount(screen, 1);
   drawLapTime(screen, 0);                                                                              
   drawTime(screen, 0);
   drawSector(screen, 1);                                                                            //starting up in sector 1
 }
}

void initText(Adafruit_SharpMem& screen, bool right) {
    
  if(!right) {
    screen.setTextColor(BLACK);
    screen.setFont(&Open_Sans_Bold_12);
    drawString(screen, "Lap ", 165, yMarginSpeedLapCount + textVerticalMargin, 1);
   
    
    screen.setTextColor(WHITE);
    drawString(screen, "time", 177, 195, 1);
    screen.setFont(&Open_Sans_Regular_12);
    drawString(screen, "Lap ", 150,195, 1);

    
    screen.setFont(&Open_Sans_Regular_12);
    drawString(screen, "Sector",  47, 270, 1);
    screen.write('r');                                                                                //må fikses så man kan ha lengre strenger


    drawString(screen, "Time",  142, 270, 1);
    screen.setFont(&Open_Sans_Bold_12);
    drawString(screen, "total",  177, 270, 1);
    
  } else if (right) {
      screen.setFont(&Open_Sans_Regular_12);
      screen.setTextColor(BLACK);
      drawString(screen, "km/h",  163, yMarginSpeedLapCount, 1);
      screen.setTextColor(WHITE);
      screen.setFont(&Open_Sans_Regular_10);
      drawString(screen, "Voltage",  170, 212, 1);                                                   //sort of waste to write both V and Voltage
      drawString(screen, "ge  ",  200, 212, 1);
      
      drawString(screen, "Current",  170, 257, 1);                                                   //-----//------ A and Current
      drawString(screen, "nt  ",  200, 257, 1);

      drawString(screen, "Energy",  177, 302, 1);                                                    //-----//------ W and Energy

      screen.setFont(&Open_Sans_Bold_18);
      drawString(screen, "V   ", 200, 200, 1);
      drawString(screen, "A   ", 200, 245, 1);
      drawString(screen, "W   ", 195, 290, 1);
  }
    
}

//INIT FUNCTIONS

void updateScreen(Adafruit_SharpMem& screen, bool right)
{
 if(right)
 {
   drawSpeed(screen, motor1Msg.buf[6]/5, motor2Msg.buf[6]/5);                                        //speed is sent from motor controllers at position 7
                                                                                                     //it is sent as km/h * 5 to utilize the measuring range
                                                                                                        
   drawGear(screen, clutchMsg.buf[3]);                                                               //clutch status aka gear is sent as 0=neutral, 1=first, 2=secound
   //drawLightIcon(screen, lightON);                                                                 //variable defined in main
   drawVoltageValue(screen, (motor1Msg.buf[2]<<8 + motor1Msg.buf[3])/10,                             //voltage uses two positions in motor controller can message
                            (motor2Msg.buf[2]<<8 + motor2Msg.buf[3])/10);                            //first position needs 8 bitshifts, and that value i batt V *10
   
   drawCurrentValue(screen, motor1Msg.buf[1]/10, motor2Msg.buf[2]/10);                               //curent is sent at posison 1 and is motor current A * 10
   drawPowerConsumption(screen, (motor1Msg.buf[4]<<8 + motor1Msg.buf[5])/10,                         //identical as voltage, just position 5 and 6
                                (motor2Msg.buf[4]<<8 + motor2Msg.buf[5])/10);                        //first position needs 8 bitshifts, and that value i used energy *10;                                                                   
   //drawRaceMode(screen, raceModeON);                                                               //variable defined in main
 
 } else if (!right) 
 {
   drawLapCount(screen, 2);
   drawLapTime(screen, 1);                                                                              
   drawTime(screen, 1000);
   drawSector(screen, sWheelMsg.buf[7]);  //sent at last position of steering wheel
   
 }
  //readCan(rxMsg, sWheelMsg);
// printCanToSerial(sWheelMsg, true);
   screen.refresh();
}

//DRAW FUNCTIONS FOR LEFT SCREEN

void drawLapCount(Adafruit_SharpMem& screen, const uint8_t& lapCount) 
{
    screen.setFont(&Open_Sans_Bold_60);                                                              //setting correct font and size
    screen.setTextColor(BLACK);                                                               
    screen.fillRect(30, yMarginSpeedLapCount-fontHeight60, 170, fontHeight60+1, WHITE);              //erase previous text
    
    char str[16];
    sprintf(str, "%02u/%02u", lapCount, maxLaps); //formating string
    drawString(screen, str, lapCountHorizontalMargin, yMarginSpeedLapCount, 1);                      //printing new text
}

void drawLapTime(Adafruit_SharpMem& screen, const int& lapTimeSeconds) {

    // format time
    int mins = lapTimeSeconds / 60;
    int secs = lapTimeSeconds % 60;


    char str[16] = {0};
    sprintf(str, "%02u:%02u", mins, secs);
    

    screen.fillRect(90, 147, 120, fontHeight45, BLACK);
    screen.setFont(&Open_Sans_Bold_45);
    screen.setTextColor(WHITE);
    drawString(screen, str, 90, 180, 1);
}

void drawTime(Adafruit_SharpMem& screen, const int& timeSeconds) {

    // format time
    int mins = timeSeconds  / 60;
    int secs = timeSeconds  % 60;


    char str[16] = {0};
    sprintf(str, "%02u:%02u", mins, secs);
    

    screen.fillRect(115, 255-fontHeight36, 100, fontHeight36, BLACK);
    screen.setFont(&Open_Sans_Bold_36);
    screen.setTextColor(WHITE);
    drawString(screen, str, 115, 255, 1);
}

void drawRectangle(Adafruit_SharpMem& screen)//, int width, int height) 
{
    screen.fillRect(45, 210, 45, 45, WHITE);
    screen.fillRect(47, 212, 41, 41, BLACK);

}

void drawSector(Adafruit_SharpMem& screen, int sector) {
    drawRectangle(screen);
    //Serial.println(sector, HEX);
    char str[8] = {0};
    sprintf(str, "%u", sector);
    screen.setFont(&Open_Sans_Bold_36);
    screen.setTextColor(WHITE);
    drawString(screen, str, 57, 245, 1);
}

//DRAW FUNCTIONS FOR RIGHT SCREEN


void drawSpeed(Adafruit_SharpMem& screen, const float& motor1speed, const float& motor2speed) 
{
    const uint8_t speedThreshold = 10;                                                                   //move to global? or argument?                                                 
    int motorSpeed = 0;
    
    screen.setFont(&Open_Sans_Bold_60);                                                                  //setting correct font and size
    screen.setTextColor(BLACK);
    screen.fillRect(40, yMarginSpeedLapCount-fontHeight60, 123, fontHeight60+1, WHITE);                  //erase previous text
      
    if (abs(motor1speed-motor2speed) > speedThreshold) {                                                 //figure out if speed sent over CANbus is ok to use
      drawString(screen, "err.", 55, yMarginSpeedLapCount, 1);                                           //err. is printed if not
    } else {
      motorSpeed = (motor1speed + motor2speed)/2;                                                        //else avrage speed is printed
      char str[16] = {0};
      sprintf(str, "%02u", motorSpeed);
      drawString(screen, str, 85, yMarginSpeedLapCount, 1);
     
    }  
}

void drawGear(Adafruit_SharpMem& screen, const char gear) {
    //Position variables
    const uint8_t xMargin = 6;
    const uint8_t yMargin = 138;
    const uint8_t width = 120;
    const uint8_t height = 45;
    
    screen.setFont(&Open_Sans_Bold_36); //sets font+color and wipes out previous text
    screen.setTextColor(WHITE);
    screen.fillRect(xMargin, yMargin, width, height, BLACK);
    
    
    switch(gear) {                                                                                       //gear desides where to place circle
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

void drawVoltageValue(Adafruit_SharpMem& screen, const double& motor1voltage, const double& motor2voltage)
{
    double voltage = (motor1voltage + motor2voltage)/2;
    char str[16];
    sprintf(str, "%-4.2f", voltage);

    screen.fillRect(145, 186, 50, fontHeight18, BLACK);
    screen.setFont(&Open_Sans_Bold_18);
    screen.setTextColor(WHITE);
    drawString(screen, str, 145, 200, 1);
}

void drawCurrentValue(Adafruit_SharpMem& screen, const double& motor1current, const double& motor2current) 
{
    double current = (motor1current + motor2current)/2;

    char str[16] = {0};
    sprintf(str, "%-4.2f", current);

    screen.setFont(&Open_Sans_Bold_18);
    screen.setTextColor(WHITE);
    screen.fillRect(145, 230, 50, fontHeight18, BLACK);
    drawString(screen, str, 145, 245, 1);
}

void drawPowerConsumption(Adafruit_SharpMem& screen, const double& motor1power, const double& motor2power) 
{
    double current = (motor1power + motor2power)/2;

    char str[16] = {0};
    sprintf(str, "%-4.2f", current);

    screen.setFont(&Open_Sans_Bold_18);
    screen.setTextColor(WHITE);
    screen.fillRect(145, 277, 50, fontHeight18, BLACK);
    drawString(screen, str, 145, 290, 1);
}

void drawRaceMode(Adafruit_SharpMem& screen, bool on) 
{
    screen.fillRect(8, 303, 7, 7, BLACK);
    if(on) {
      screen.setFont(&Open_Sans_Bold_10);
      screen.setTextColor(WHITE);
      drawString(screen, "R", 8, 310, 1);      
    }
}
