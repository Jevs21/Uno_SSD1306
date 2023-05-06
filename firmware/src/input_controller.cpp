#include <Arduino.h>
#include <Joystick.h>
#include "../include/input_controller.h"

#define X_PIN A0
#define Y_PIN A1
#define S_PIN 2


InputController::InputController(uint8_t xPin, uint8_t yPin, uint8_t sPin) {
  // Initialize joystick here
  // joystick(xPin, yPin, sPin);
  // _joystick(xPin, yPin, sPin);
  Joystick joystick(X_PIN, Y_PIN, S_PIN);
}

void InputController::print(const char* xLabel, const char* yLabel, const char* sLabel, int minDelay, int maxDelay) {
  // Print joystick values here
}

void InputController::getJoystickValues(int* arr) {
  int xVal = analogRead(X_PIN);
  int yVal = analogRead(Y_PIN);
  bool sVal = digitalRead(S_PIN);

  if (xVal < 400) {
    arr[0] = -1;
  } else if (xVal > 600) {
    arr[0] = 1;
  } else {
    arr[0] = 0;
  }
  if (yVal < 400) {
    arr[1] = -1;
  } else if(yVal > 600) {
    arr[1] = 1;
  } else {
    arr[1] = 0;
  }
  // int values[3] = {xVal, yVal, sVal};
  // return values;
  // arr[0] = xVal;
  // arr[1] = yVal;
  arr[2] = (sVal) ? 0 : 1;
}
