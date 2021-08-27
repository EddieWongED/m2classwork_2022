#include <Arduino.h>

const int OUTPUT_PIN = 13;

void setup()
{
    pinMode(OUTPUT_PIN, OUTPUT);
}

void S()
{
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(OUTPUT_PIN, HIGH);
        delay(500);
        digitalWrite(OUTPUT_PIN, LOW);
        delay(300);
    }
}

void O()
{
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(OUTPUT_PIN, HIGH);
        delay(1500);
        digitalWrite(OUTPUT_PIN, LOW);
        delay(300);
    }
}

void loop()
{
    S();
    O();
    S();
    delay(300);
}