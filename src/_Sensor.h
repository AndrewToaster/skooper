#include "SkpChip.h"

void setup()
{
    while (!skp_tryConnect())
    {
        Serial.println("Retrying!");
        delay(5000);
    }
}

void loop()
{
    
}