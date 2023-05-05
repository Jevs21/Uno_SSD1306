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

  // int values[3] = {xVal, yVal, sVal};
  // return values;
  arr[0] = xVal;
  arr[1] = yVal;
  arr[2] = sVal;
}
