/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Joystick.h>

// #include "include/display_funcs.h"
#include "include/input_controller.h"
#include "include/display_tests.h"
// #include "include/menu.h"

int inputs[3] = {0, 0, 0};

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


static const unsigned char PROGMEM icon_note_bmp[] = {
0b00000000, 0b00000000, 0b00000000, 0b00000000, 
0b00000000, 0b00000000, 0b00000000, 0b11100000, 
0b00000000, 0b00000000, 0b00000011, 0b11100000, 
0b00000000, 0b00000000, 0b00000111, 0b11100000, 
0b00000000, 0b00000000, 0b00011111, 0b01100000, 
0b00000000, 0b00000000, 0b00111110, 0b01100000, 
0b00000000, 0b00000000, 0b11111000, 0b01100000, 
0b00000000, 0b00000001, 0b11110000, 0b01100000, 
0b00000000, 0b00000111, 0b11000000, 0b01100000, 
0b00000000, 0b00001111, 0b10000000, 0b01100000, 
0b00000000, 0b00001100, 0b00000000, 0b01100000, 
0b00000000, 0b00001100, 0b00000000, 0b01100000, 
0b00000000, 0b00001100, 0b00000000, 0b01100000, 
0b00000000, 0b00001100, 0b00000000, 0b01100000, 
0b00000000, 0b00001100, 0b00001111, 0b01100000, 
0b00000000, 0b00001100, 0b00011111, 0b11100000, 
0b00000000, 0b00001100, 0b00111111, 0b11100000, 
0b00000000, 0b00001100, 0b01111111, 0b11100000, 
0b00000000, 0b00001100, 0b01111111, 0b11100000, 
0b00000000, 0b00001100, 0b01111111, 0b11100000, 
0b00000000, 0b00001100, 0b01111111, 0b11100000, 
0b00000000, 0b00001100, 0b00111111, 0b11000000, 
0b00000001, 0b11101100, 0b00011111, 0b10000000, 
0b00000011, 0b11111100, 0b00001111, 0b00000000, 
0b00000111, 0b11111100, 0b00000000, 0b00000000, 
0b00001111, 0b11111100, 0b00000000, 0b00000000, 
0b00001111, 0b11111100, 0b00000000, 0b00000000, 
0b00001111, 0b11111100, 0b00000000, 0b00000000, 
0b00001111, 0b11111100, 0b00000000, 0b00000000, 
0b00000111, 0b11111000, 0b00000000, 0b00000000, 
0b00000011, 0b11110000, 0b00000000, 0b00000000, 
0b00000001, 0b11100000, 0b00000000, 0b00000000, 
};

static const unsigned char PROGMEM icon_demon_bmp[] = {
0b00000000, 0b00000000, 0b00000000, 0b00000000, 
0b00000000, 0b00000100, 0b01000000, 0b00000000, 
0b00000000, 0b00001000, 0b00100000, 0b00000000, 
0b00000000, 0b00001000, 0b00100000, 0b00000000, 
0b00000000, 0b00001100, 0b01100000, 0b00010000, 
0b00000000, 0b00001111, 0b11100000, 0b00111000, 
0b00000000, 0b00000111, 0b11000000, 0b00111000, 
0b00000100, 0b00000101, 0b01000000, 0b01111100, 
0b00010100, 0b00000111, 0b11000000, 0b00010000, 
0b01010100, 0b00000011, 0b10000000, 0b00010000, 
0b01010100, 0b00000011, 0b11000000, 0b00010000, 
0b01011000, 0b00000111, 0b11100000, 0b00010000, 
0b01101000, 0b00001111, 0b11100001, 0b10100000, 
0b00001000, 0b00011111, 0b11110010, 0b01100000, 
0b00001111, 0b11111111, 0b11110010, 0b00100000, 
0b00001111, 0b11110111, 0b11111010, 0b01100000, 
0b00000100, 0b00000111, 0b11011001, 0b11100000, 
0b00000010, 0b00000111, 0b11011000, 0b00100000, 
0b00000010, 0b00000111, 0b11011000, 0b00100000, 
0b00000010, 0b00000111, 0b11011000, 0b01000000, 
0b00000001, 0b00000111, 0b11111000, 0b01000000, 
0b00000001, 0b00000111, 0b11110000, 0b10000000, 
0b00000001, 0b00001111, 0b11101111, 0b00000000, 
0b00000000, 0b10001111, 0b11100000, 0b00000000, 
0b00000000, 0b10011110, 0b11000000, 0b00000000, 
0b00000000, 0b10011100, 0b11000000, 0b00000000, 
0b00000000, 0b01011000, 0b11100000, 0b00000000, 
0b00000000, 0b01001100, 0b01100000, 0b00000000, 
0b00000000, 0b01000100, 0b00100000, 0b00000000, 
0b00000000, 0b00001100, 0b01100000, 0b00000000, 
0b00000000, 0b00011100, 0b11100000, 0b00000000, 
0b00000000, 0b00000000, 0b00000000, 0b00000000, 
};

static const unsigned char PROGMEM icon_warning_bmp[] = {
0b00000000, 0b00000000, 0b00000000, 0b00000000, 
0b00000000, 0b00000000, 0b00000000, 0b00000000, 
0b00000000, 0b00000111, 0b00000000, 0b00000000, 
0b00000000, 0b00001000, 0b10000000, 0b00000000, 
0b00000000, 0b00010000, 0b11100000, 0b00000000, 
0b00000000, 0b00010000, 0b01110000, 0b00000000, 
0b00000000, 0b00100000, 0b01110000, 0b00000000, 
0b00000000, 0b00100000, 0b00111000, 0b00000000, 
0b00000000, 0b01000000, 0b00111000, 0b00000000, 
0b00000000, 0b01000111, 0b00011100, 0b00000000, 
0b00000000, 0b10001111, 0b10011100, 0b00000000, 
0b00000000, 0b10001111, 0b10001110, 0b00000000, 
0b00000001, 0b00001111, 0b10001110, 0b00000000, 
0b00000001, 0b00001111, 0b10000111, 0b00000000, 
0b00000010, 0b00001111, 0b10000111, 0b00000000, 
0b00000010, 0b00001111, 0b10000011, 0b10000000, 
0b00000100, 0b00001111, 0b10000011, 0b10000000, 
0b00000100, 0b00001111, 0b10000001, 0b11000000, 
0b00001000, 0b00000111, 0b00000001, 0b11000000, 
0b00001000, 0b00000111, 0b00000000, 0b11100000, 
0b00010000, 0b00000111, 0b00000000, 0b11100000, 
0b00010000, 0b00000111, 0b00000000, 0b01110000, 
0b00100000, 0b00000000, 0b00000000, 0b01110000, 
0b00100000, 0b00000111, 0b00000000, 0b00111000, 
0b01000000, 0b00001111, 0b10000000, 0b00111000, 
0b01000000, 0b00001111, 0b10000000, 0b00011100, 
0b10000000, 0b00000111, 0b00000000, 0b00011100, 
0b10000000, 0b00000000, 0b00000000, 0b00001110, 
0b10000000, 0b00000000, 0b00000000, 0b00001110, 
0b11111111, 0b11111111, 0b11111111, 0b11111110, 
0b00011111, 0b11111111, 0b11111111, 0b11111100, 
0b00000000, 0b00000000, 0b00000000, 0b00000000, 
};

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
InputController inputController(0, 1, 2);
// Menu menu();

void displaySetup () {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
}

void setup() {
  Serial.begin(9600);
  displaySetup();

}

int menuIndY = 0;
int menuIndX = 0;



void drawMainMenu(int x) {
  display.clearDisplay();

  if (x == 0) {
    display.drawBitmap(
    (display.width()  - 32) / 2,
    (display.height() - 32) / 2,
    icon_note_bmp, 32, 32, 1);
  } else if (x == 1) {
    display.drawBitmap(
    (display.width()  - 32) / 2,
    (display.height() - 32) / 2,
    icon_demon_bmp, 32, 32, 1);
  } else if (x == 2) {
    display.drawBitmap(
    (display.width()  - 32) / 2,
    (display.height() - 32) / 2,
    icon_warning_bmp, 32, 32, 1);
  }
  
  display.display();
}

void loop() {
  inputController.getJoystickValues(inputs);

  // Draw
  drawMainMenu(menuIndX);
  delay(100);

  // Control
  if (inputs[2]) {
    // testdrawcircle(display);
    // menu.debugPrint();
    // testdrawbitmap();
    Serial.println("Clicked!");
  } else {
    if (inputs[0] > 0) {
      menuIndX += 1;
      if (menuIndX > 2) {
        menuIndX = 0;
      }
      Serial.println("Going right.");
    } else if (inputs[0] < 0) {
      menuIndX -= 1;
      if (menuIndX < 0) {
        menuIndX = 2;
      }
      Serial.println("Going left.");
    }
    if (inputs[1] > 0) {
      Serial.println("Going down.");
    } else if (inputs[1] < 0) {
      Serial.println("Going up.");
    }
    // testPrintInputs(display, inputs[0], inputs[1], inputs[2]);
  }
}


