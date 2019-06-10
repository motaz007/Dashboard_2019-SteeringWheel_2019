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




//TEST FUNCTIONS

void testfillrect(Adafruit_SharpMem disp) {
  uint8_t color = 1;
  for (uint8_t i=0; i<disp.height()/2; i+=3) {
    // alternate colors
    disp.fillRect(i, i, disp.width()-i*2, disp.height()-i*2, color%2);
    disp.refresh();
    color++;
  }
}

   
void testdrawrect(Adafruit_SharpMem disp) {
  for (uint8_t i=0; i<disp.height()/2; i+=2) {
    disp.drawRect(i, i, disp.width()-2*i, disp.height()-2*i, BLACK);
    disp.refresh();
  }
}
