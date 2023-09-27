#ifndef SKOOPER_SKPCOMMON_H
#define SKOOPER_SKPCOMMON_H

#include <WString.h>

typedef struct skp_data
{
    float temp;
    float hum;
    float pres;
} skp_data;

bool skp_tryConnect();
void skp_initSerial();
String skp_serializeData(const skp_data *data);

extern const char *MIME_HTML;
extern const char *MIME_PLAIN;
extern const char *MIME_JSON;

#endif
