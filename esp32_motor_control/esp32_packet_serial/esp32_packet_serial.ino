#include <RoboClaw.h>

RoboClaw roboclaw(&Serial2, 10000);

void setup() {
  roboclaw.begin(38400);
}

void loop() {
  roboclaw.ForwardM1(0x80, 64);
  delay(2000);
  roboclaw.ForwardM1(0x80, 0);
  delay(2000);

  roboclaw.ForwardM2(0x80, 64);
  delay(2000);
  roboclaw.ForwardM2(0x80, 0);
  delay(2000);
}
