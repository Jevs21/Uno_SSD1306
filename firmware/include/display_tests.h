#ifndef DISPLAY_TESTS_H
#define DISPLAY_TESTS_H

// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
// void testdrawline(void);

// void testdrawrect(void);

// void testfillrect(void);

void testPrintInputs(Adafruit_SSD1306 &display, int x, int y, int s);

void testdrawcircle(Adafruit_SSD1306 &display);

// void testfillcircle(void);

// void testdrawroundrect(void);

// void testfillroundrect(void);

// void testdrawtriangle(void);

// void testfilltriangle(void);

// void testdrawchar(void);

// void testdrawstyles(void);

// void testscrolltext(void);

// void testdrawbitmap(void);

// void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h);

#endif // DISPLAY_TESTS_H