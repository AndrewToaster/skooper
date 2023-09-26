#include "SkpChip.h"
#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    while (!skp_tryConnect())
    {
        Serial.println("Retrying!");
        delay(5000);
    }
}

void loop()
{
    delay(100);
}