#ifndef SKOOPER_SKPUTIL_H
#define SKOOPER_SKPUTIL_H

#include <WString.h>

#define SKP_LOG_LEVEL_ERROR (1)
#define SKP_LOG_LEVEL_INFO (SKP_LOG_LEVEL_ERROR + 1)
#define SKP_LOG_LEVEL_DEBUG (SKP_LOG_LEVEL_INFO + 1)

#ifndef SKP_LOG_LEVEL
#define SKP_LOG_LEVEL SKP_LOG_LEVEL_INFO
#endif

#define __SKP_LOGFIFY(prefix, fmt, ...) { _skp_logf((prefix), __FILE__, __PRETTY_FUNCTION__, __LINE__); Serial.printf((fmt), __VA_ARGS__); }

// Your average macro ~~abuse~~ usage
#if SKP_LOG_LEVEL >= SKP_LOG_LEVEL_ERROR  
#define skp_log_error(message) _skp_log("ERROR", (message), __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define skp_logf_error(fmt, ...) __SKP_LOGFIFY("ERROR", (fmt), __VA_ARGS__)
#else
#define skp_log_error(_)
#define skp_logf_error(_, ...)
#endif

#if SKP_LOG_LEVEL >= SKP_LOG_LEVEL_INFO
#define skp_log_info(message) _skp_log("INFO", (message), __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define skp_logf_info(fmt, ...) __SKP_LOGFIFY("INFO", (fmt), __VA_ARGS__)
#else
#define skp_log_info(_)
#define skp_logf_info(_, ...)
#endif

#if SKP_LOG_LEVEL >= SKP_LOG_LEVEL_DEBUG
#define skp_log_debug(message) _skp_log("DEBUG", (message), __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define skp_logf_debug(fmt, ...) __SKP_LOGFIFY("DEBUG", (fmt), __VA_ARGS__)
#else
#define skp_log_debug(_)
#define skp_logf_debug(_, ...)
#endif

#define skp_halt() { skp_log_info("Halted"); _skp_halt(); }
#define skp_arr_size(arr, type) (sizeof(arr) / sizeof(type))

// Macro fuckery
#define __SKP_STRIFY(a) #a
#define SKP_STRIFY(a) __SKP_STRIFY(a)

void _skp_log(const char *prefix, const char *message, const char *file, const char *func, const int line);
void _skp_log(const char *prefix, const String& message, const char *file, const char *func, const int line);
void _skp_logf(const char *prefix, const char *file, const char *func, const int line);
void _skp_halt();
bool skp_str2uint(String s, int *result);
bool skp_str2float(String s, float *result);

#endif