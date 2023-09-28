#define SKP_LOG_LEVEL 3

#include <Arduino.h>
#include "SkpCommon.h"

// Little bit of macro fuckery to 'Rename' the main functions
#define setup _setup
#define loop _loop

//#define TYPE_SERVER
#define TYPE_SENSOR

#ifdef TYPE_SERVER
#define _TYPE "SERVER"
#include "_Server.h"
#else
#ifndef TYPE_SENSOR
#warning Neither TYPE_SERVER nor TYPE_SENSOR was defined, assuming TYPE_SENSOR
#endif
#define _TYPE "SENSOR"
#include "_Sensor.h"
#endif

// Now we can undefine the macros so we can use the normal main function names
#undef setup
#undef loop

void setup() 
{
    skp_initSerial();
    Serial.println("Running as " _TYPE);

    while (!skp_tryConnect())
    {
        Serial.println("Retrying!");
        delay(5000);
    }

    // Call the setup function of either _Server.h or _Sensor.h
    _setup();
}

void loop()
{
    // Call the setup function of either _Server.h or _Sensor.h
    _loop();
}