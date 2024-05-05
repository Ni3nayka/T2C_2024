#include "GY-25.h"

GY25 gy25(A0, A1); // RX, TX

void setup() {
  Serial.begin(9600);
  gy25.setup();
}

long int t = 0;

void loop() {
  gy25.update();
  if (t < millis()) {
    gy25.print();
    t = millis() + 100;
  }
}
