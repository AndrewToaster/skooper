#ifndef SKOOPER_SKPUTIL_H
#define SKOOPER_SKPUTIL_H

// Your average macro ~~abuse~~ usage
#define skp_log_error(message) _skp_log_error((message), __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define skp_log_errorS(message) _skp_log_error((message).c_str(), __FILE__, __PRETTY_FUNCTION__, __LINE__)

void _skp_log_error(const char *message, const char *file, const char *func, const int line);
void skp_halt();

#endif