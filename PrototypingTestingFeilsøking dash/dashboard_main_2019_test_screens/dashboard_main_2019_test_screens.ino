/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * Code for the dashboard of the 2019 car
 */

/*------------------ Include Headerfiles ------------------*/
#include "can.h"
#include "graphics.h"
#include "helpers.h"
#include <Snooze.h>


/*-------------------- COMM PARAMETERS --------------------*/

// LIGHTS
#define PIN_BACKLIGHT   35
#define PIN_FRONTLIGHT   36

// BUTTONS
#define PIN_BRAKE_ENABLED    26
#define PIN_LIGHT_ENABLE     28
#define PIN_MAIN_SWITCH      37  // is this needed? everything turns off with the emergency switch anyways...
#define PIN_HAZARD_LIGHT     16
#define PIN_GEAR_2           29  // up
#define PIN_GEAR_1           25  // down
#define PIN_RESET            30
#define PIN_GEAR_AUTO_MANUAL 12

// precalculated bitmask (1<<bp) (bp = bit position) is supposed to be a little bit faster
#define GEAR_2_bp 1
#define GEAR_2_bm (1<<GEAR_2_bp)
#define GEAR_1_bp 0
#define GEAR_1_bm (1<<GEAR_1_bp)
#define GEAR_AUTO_MANUAL_bp 2
#define GEAR_AUTO_MANUAL_bm (1<<GEAR_AUTO_MANUAL_bp)

// RIGHT SCREEN
#define S1_SCK  13
#define S1_MOSI 11
#define S1_CS   10

// LEFT SCREEN
#define S2_SCK  32
#define S2_MOSI 21
#define S2_CS   31

//snooze

//SnoozeSPI spi;
//SnoozeBlock config(spi);


static CAN_message_t txMsg, rxMsg;

Adafruit_SharpMem screen1(S1_SCK, S1_MOSI, S1_CS, WIDTH, HEIGHT);
Adafruit_SharpMem screen2(S2_SCK, S2_MOSI, S2_CS, WIDTH, HEIGHT);
enum ORIENTATION { UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 };

/* INIT FUNCTIONS */
void initScreen1() {
    screen1.begin();
    screen1.setRotation(ORIENTATION::UP);
    screen1.setFont(&FreeMono9pt7b);
    screen1.setTextColor(BLACK, WHITE);
    clearScreen(screen1);

    screen1.refresh();
}

void initScreen2() {
    screen2.begin();
    screen2.setRotation(ORIENTATION::DOWN);
    screen2.setFont(&FreeMono9pt7b);
    screen2.setTextColor(BLACK, WHITE);
    clearScreen(screen2);

    screen2.refresh();
}

void initScreensContent() {
    // Draw background only once
    drawBackground(screen1, true);
    drawBackground(screen2, false);

    const char mins[] = "mins";
    screen2.setFont(&FreeMono9pt7b);
    drawString(screen2, "60", 260, 163, 1);

    const char kmh[] = "km/h";
    screen1.setFont(&FreeMono9pt7b);
    drawString(screen1, "14", 90, 170, 1);
    drawSpeed(screen1, 15, 13);

    

    // setup for motor states
    const char m1[] = "M1:";
    const char m2[] = "M2:";
    screen1.setFont(&FreeMono9pt7b);
    
    
    

    // setup for motor temps
    const char t1[] = "T1:";
    const char t2[] = "T2:";
    screen2.setFont(&FreeMono12pt7b);
    drawString(screen2, t1, 204, 201, 1);
    drawString(screen2, t2, 204, 221, 1);


    // setup for motor clutch
    const char c1[] = "C1:";
    const char c2[] = "C2:";
    screen1.setFont(&FreeMono12pt7b);
    drawString(screen1, c1, 6, 201, 1);
    drawString(screen1, c2, 6, 221, 1);


    // setup for total energy
    const char e1[] = "kJ1:";
    const char e2[] = "kJ2:";
    screen2.setFont(&FreeMono9pt7b);
    drawString(screen2, e1, 192, 35, 1);
    drawString(screen2, e2, 192, 55, 1);
}






/*----------------------- SETUP -------------------------*/
void setup() {
  // put your setup code here, to run once:
  SIM_CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(15) | // Core/system clock 4MHz
                SIM_CLKDIV1_OUTDIV2(15) | // peripheral clock
                SIM_CLKDIV1_OUTDIV3(15) | // CANbus Clock
                SIM_CLKDIV1_OUTDIV4(15)); // flash clock
  initSerial();
  initCAN();
  initScreen1();
  initScreen2();
  initScreensContent();
  screen1.refresh();
}

/*-------------------- MAIN LOOP --------------------*/

void loop() {
  String drawS;
  
  readCan(rxMsg);
  printCanToSerial(rxMsg);
  drawS = convertCanToString(rxMsg);
  int i = rxMsg.buf[3];
  int j = rxMsg.buf[2];
  drawSpeed(screen1, i, j);
  //clearScreen(screen1);
  //drawString2(screen1, drawS, 0, 200, 0);
  //drawBackground(screen1, true);
  screen1.refresh();
}
