#include <Arduino.h>

volatile long temp, counter = 0;
const int INPUT_PIN1 = 2;
const int INPUT_PIN2 = 3;

void ai0()
{
  if (digitalRead(INPUT_PIN2) == LOW)
  {
    counter++;
  }
  else
  {
    counter--;
  }
}

void ai1()
{
  if (digitalRead(INPUT_PIN1) == LOW)
  {
    counter--;
  }
  else
  {
    counter++;
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(INPUT_PIN1, INPUT_PULLUP);
  pinMode(INPUT_PIN2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(2), ai0, RISING);

  attachInterrupt(digitalPinToInterrupt(3), ai1, RISING);
}

void loop()
{
  if (counter != temp)
  {
    Serial.println(counter);
    temp = counter;
  }
}