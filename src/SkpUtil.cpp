#include "SkpUtil.h"

// A straight call to printf basically
void _skp_log_error(const char *message, const char *file, const char *func, const int line)
{
    Serial.printf("[Error] %s at %s:%i > %s", func, file, line, message);
}