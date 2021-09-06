#include <Arduino.h>
#include "../lib/Encoder/Encoder.h"

const int INPUT_PIN1 = 2;
const int INPUT_PIN2 = 3;

Encoder myEnc(INPUT_PIN1, INPUT_PIN2);

void setup() {
  Serial.begin(9600);
}

long oldPosition  = -999;

void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
}