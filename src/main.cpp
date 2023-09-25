#include <Arduino.h>

#if defined(SERVER)
#include "_Server.h"
#else
#ifndef SENSOR
#warning Neither SERVER nor SENSOR was defined, assuming SENSOR
#endif
#include "_Sensor.h"
#endif