#ifndef SKOOPER_SKPWIFICOMMON_H
#define SKOOPER_SKPWIFICOMMON_H

typedef struct skp_data
{
    float temp;
    float hum;
    //float pres;
} skp_data;

bool skp_tryConnect();

#endif
