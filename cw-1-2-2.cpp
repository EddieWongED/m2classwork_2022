#include <Arduino.h>

const int OUTPUT_PIN = 9;
int brightness = 255;
String buffer = "";

void setup()
{
  pinMode(OUTPUT_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {
    char readByte = Serial.read();
    if ('0' <= readByte && readByte <= '9')
    {
      buffer += readByte;
      Serial.print(readByte);
    }
    else if (readByte == '\n')
    {
      brightness = buffer.toInt();
      buffer = "";
      Serial.println();
      Serial.print("Updated to ");
      Serial.println(brightness);
    }
  }
  analogWrite(OUTPUT_PIN, brightness);
}