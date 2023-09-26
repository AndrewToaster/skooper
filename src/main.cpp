#include <Arduino.h>

#define TYPE_SERVER

#if defined(TYPE_SERVER)
#include "_Server.h"
#else
#ifndef TYPE_SENSOR
#warning Neither TYPE_SERVER nor TYPE_SENSOR was defined, assuming TYPE_SENSOR
#endif
#include "_Sensor.h"
#endif