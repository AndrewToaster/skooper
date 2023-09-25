#ifndef SKOOPER_SKPUTIL_H
#define SKOOPER_SKPUTIL_H

#include <HardwareSerial.h>

// Your average macro ~~abuse~~ usage
#define skp_log_error(message) _skp_log_error(message, __FILE__, __PRETTY_FUNCTION__, __LINE__)

void _skp_log_error(const char *message, const char *file, const char *func, const int line);

#endif