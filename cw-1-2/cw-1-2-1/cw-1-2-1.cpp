#include <Arduino.h>

const int INPUT_PIN = 2;
const int OUTPUT_PIN = 13;

void setup()
{
    pinMode(INPUT_PIN, INPUT_PULLUP);
    pinMode(OUTPUT_PIN, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    static bool on = false;
    int val = digitalRead(INPUT_PIN);
    if (!val)
    {
        on = true;
    }
    if (on)
    {
        digitalWrite(OUTPUT_PIN, HIGH);
        delay(500);
        digitalWrite(OUTPUT_PIN, LOW);
        delay(500);
        on = false;
    }
}