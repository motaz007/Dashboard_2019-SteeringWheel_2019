/* SEM 2019
 * by Ludvik Rønning & Jørgen Wisløff
 * 
 */

#include "graphics.h"

void clearScreen(Adafruit_SharpMem& screen) {
    screen.clearDisplay();
}

/*void drawBackground(Adafruit_SharpMem& screen, bool right) {
    int X = 0,
        Y = 0;

    if (right) {
        screen.drawXBitmap(X, Y, final_v2_right_bits, WIDTH, HEIGHT, BLACK);
    }
    else {
        screen.drawXBitmap(X, Y, final_v2_left_bits, WIDTH, HEIGHT, BLACK);
    }
}*/

void drawString(Adafruit_SharpMem& screen, const char * str, int x, int y, int size) {
    screen.setCursor(x, y);
    screen.setTextSize(size);

    for (unsigned int i = 0; i < sizeof(str)/sizeof(str[0]) + 1; ++i) {
        screen.write(str[i]);  // `.write` moves cursor for every character
    }
}

void initScreen1() {
    screen1.begin();
    screen1.setRotation(ORIENTATION::DOWN);
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
