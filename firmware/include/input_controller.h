#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <Joystick.h>

class InputController {
public:
  InputController(uint8_t xPin, uint8_t yPin, uint8_t sPin);
  void print(const char* xLabel, const char* yLabel, const char* sLabel, int minDelay, int maxDelay);
  void getJoystickValues(int* arr);
};

#endif // JOYSTICK_H
