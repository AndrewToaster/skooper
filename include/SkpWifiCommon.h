#ifndef SKOOPER_SKPWIFICOMMON_H
#define SKOOPER_SKPWIFICOMMON_H

#include <WString.h>

typedef struct skp_data
{
    float temp;
    float hum;
    //float pres;
} skp_data;

bool skp_tryConnect();

String skp_serializeData(const skp_data *data);


#endif
