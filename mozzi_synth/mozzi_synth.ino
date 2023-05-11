#include <Arduino.h>
// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <MozziGuts.h>
#include <Oscil.h> // oscillator class
#include <tables/sin2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define KEY_PIN_1 2
#define KEY_PIN_2 0
#define KEY_PIN_3 4
#define KEY_PIN_4 16
#define KEY_PIN_5 17
#define KEY_PIN_6 5
#define KEY_PIN_7 18
#define KEY_PIN_8 19

#define KNOB_PIN_1 A0
#define KNOB_PIN_2 A1
#define KNOB_PIN_3 A2
#define KNOB_PIN_4 A3

#define SPEAKER_PIN 9
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> oscilSin(SIN2048_DATA);
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> oscilSaw(SAW2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> oscilSquare(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> oscilMod(SIN2048_DATA);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte colPins[COLS] = {11, 10, 8, 7};
byte rowPins[ROWS] = {6, 5, 4, 3 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int frequencies[ROWS][COLS] = {
  {262, 294, 330, 349},
  {392, 440, 494, 523},
  {587, 659, 698, 784},
  {880, 988, 1047, 1175}
};

float freq, amp, modFreq;
int waveform;
bool keyIsPressed = false;

void setup(){
  Serial.begin(115200);
  startMozzi(); // start Mozzi
}

// void updateControl(){
//   char key = keypad.getKey();
  
//   if (key) {
//     keyIsPressed = true;
//     for (byte i = 0; i < ROWS; i++) {
//       for (byte j = 0; j < COLS; j++) {
//         if (keys[i][j] == key) {
//           freq = frequencies[i][j];
//         }
//       }
//     }
//   } else {
//     keyIsPressed = false;
//   }

//   int knob1Val = analogRead(KNOB_PIN_1);
//   amp = map(knob1Val, 0, 1023, 0, 255) / 255.0;

//   int knob2Val = analogRead(KNOB_PIN_2);
//   waveform = map(knob2Val, 0, 1023, 0  , 2);

//   int knob3Val = analogRead(KNOB_PIN_3);
//   modFreq = map(knob3Val, 0, 1023, 0, 500);
// }

// int updateAudio(){
//   if (keyIsPressed) {
//     if (waveform == 0) {
//       oscilSin.setFreq(freq + oscilMod.next() * modFreq);
//       return oscilSin.next() * amp * 128.0;
//     } else if (waveform == 1) {
//       oscilSaw.setFreq(freq + oscilMod.next() * modFreq);
//       return oscilSaw.next() * amp * 128.0;
//     } else { // waveform == 2
//       oscilSquare.setFreq(freq + oscilMod.next() * modFreq);
//       return oscilSquare.next() * amp * 128.0;
//     }
//   } else {
//     return 0;
//   }
// }

// void loop(){
//   updateControl();
//   audioHook();
// }

void updateControl(){
  char key = keypad.getKey();
  if (key) {
    for (byte i = 0; i < ROWS; i++) {
      for (byte j = 0; j < COLS; j++) {
        if (keys[i][j] == key) {
          freq = frequencies[i][j];
          keyIsPressed = true;
        }
      }
    }
  } else {
    keyIsPressed = false;
  }


  int knob1Val = mozziAnalogRead(KNOB_PIN_1);
  amp = map(knob1Val, 0, 1023, 0, 255);


  int knob2Val = mozziAnalogRead(KNOB_PIN_2);
  waveform = map(knob2Val, 0, 1023, 0, 2);

  int knob3Val = mozziAnalogRead(KNOB_PIN_3);
  modFreq = map(knob3Val, 0, 1023, 0, 500);

  Serial.print("Frequency: ");
  Serial.print(freq);
  Serial.print(" Amplitude: ");
  Serial.print(amp);
  Serial.print(" Waveform: ");
  Serial.print(waveform);
  Serial.print(" Modulation Frequency: ");
  Serial.println(modFreq);
}

int updateAudio() {
  if (keyIsPressed) {
    if (waveform == 0) {
      // oscilSin.setFreq(freq + oscilMod.next() * modFreq);
      oscilSin.setFreq(freq + oscilMod.next());
      return oscilSin.next() * amp;
    } else if (waveform == 1) {
      // oscilSaw.setFreq(freq + oscilMod.next() * modFreq);
      oscilSaw.setFreq(freq + oscilMod.next());
      return oscilSaw.next() * amp;
    } else {
      // oscilSquare.setFreq(freq + oscilMod.next() * modFreq);
      oscilSquare.setFreq(freq + oscilMod.next());
      return oscilSquare.next() * amp;
    }
  }
  return 0;  // return 0 when no key is pressed
}

void loop() {
  updateControl();
  audioHook();
  delay(10);
}



