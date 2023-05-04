#include <Joystick.h>

Joystick joystick(A0, A1, 2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  joystick.print("X: ", "\tY: ", "\tZ: ", 0, 100);
  Serial.println();
}
