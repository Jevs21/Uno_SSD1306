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

#define X_PIN A0
#define Y_PIN A1
#define S_PIN 2

int xVal = 0;
int yVal = 0;
bool sVal = false;

Joystick joystick(X_PIN, Y_PIN, S_PIN);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };


int paddleX = 0;
int paddleY = SCREEN_HEIGHT - 4;
int paddleWidth = 20;
int paddleHeight = 4;

int ballX = 0;
int ballY = 0;
int ballRad = 2;
int ballXSpeed = 2;
int ballYSpeed = 2;

typedef struct {
  int x;
  int y;
  int isHit;
} Brick;

int num_bricks = 20;
int brickWidth = 6;
int brickHeight = 4;

Brick bricks[20] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
};

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  int cur_x, cur_y = 20;
  // Initialize bricks
  for(int i=0; i<num_bricks; i++) {
    bricks[i] = { cur_x, cur_y, 0 };
    cur_x += brickWidth;
    if (cur_x + 6 >= SCREEN_WIDTH - 20) {
      cur_x = 20;
      cur_y += brickHeight + 2;
    }
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

  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);

}

// int* brickX = {}



void loop() {
  // Get inputs
  xVal = analogRead(X_PIN);
  yVal = analogRead(Y_PIN);
  sVal = digitalRead(S_PIN);
  // Convert raw inputs into basic actions
  int xDir, yDir = 0;
  bool isClicking = false;
  if (xVal < 300) {
    xDir = -1;
  } else if (xVal > 700) {
    xDir = 1;
  } else {
    xDir = 0;
  }
  if (yVal < 300) {
    yDir = -1;
  } else if (yVal > 700) {
    yDir = 1;
  } else {
    yDir = 0;
  }
  if (sVal == 0) {
    isClicking = true;
  } else {
    isClicking = false;
  }


  // Clear display
  display.clearDisplay();


  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("X: ");
  display.print(xVal);
  display.print("\tY: ");
  display.print(yVal);
  display.print("\tS: ");
  display.print(sVal);

  // Draw bricks
  for (int i=0; i < num_bricks; i++) {
    if (!bricks[i].isHit) {
      display.fillRect(bricks[i].x, bricks[i].y, brickWidth, brickHeight, SSD1306_WHITE);
    }
  }
  // Draw ball
  display.fillCircle(ballX, ballY, ballRad, SSD1306_WHITE);
  // Draw paddle
  display.fillRect(paddleX, paddleY, paddleWidth, paddleHeight, SSD1306_WHITE);
  
  display.display();

  // Move ball
  ballX += ballXSpeed;
  ballY += ballYSpeed;
  // Check for ball collisions
  if ( 
    // Collision with paddle
    ballX >= paddleX 
    && ballX <= paddleX + paddleWidth
    && ballY >= paddleY
    && ballY <= paddleY + paddleHeight) {
      ballYSpeed = ballYSpeed * -1;
      
  } else {
    if (ballX <= 0 || ballX >= SCREEN_WIDTH - ballRad) {
      ballXSpeed = ballXSpeed * -1;
    }
    if (ballY <= 0 || ballY >= SCREEN_HEIGHT - ballRad) {
      ballYSpeed = ballYSpeed * -1;
    }

    // Check brick collisions
    for(int i=0; i < num_bricks; i++) {
      if (!bricks[i].isHit) {
        if (
          ballX >= bricks[i].x &&
          ballX <= bricks[i].x + brickWidth && 
          ballY <= bricks[i].y &&
          ballY <= bricks[i].y + brickHeight
          ) {
            if (ballX >= bricks[i].x &&
              ballX <= bricks[i].x + brickWidth) {
                ballXSpeed = ballXSpeed * -1;
            }
            if (ballY <= bricks[i].y &&
                ballY <= bricks[i].y + brickHeight) {
                ballYSpeed = ballYSpeed * -1;
            }
            bricks[i].isHit = 1;
          }
      }
    }
  }

  // Move paddle
  if (xDir == -1) {
    paddleX -= 3;
    if (paddleX < 0) {
      paddleX = 0;
    }
  } else if (xDir == 1) {
    paddleX += 3;
    if (paddleX > SCREEN_WIDTH - paddleWidth) {
      paddleX = SCREEN_WIDTH - paddleWidth;
    }
  }

  delay(10);
}
