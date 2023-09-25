#ifndef SERVER_CON_H
#define SERVER_CON_H

#include <WString.h>

typedef struct skp_data
{
    float temp;
    float hum;
    float pres;
} skp_data;

int skp_sendData(const skp_data *data);
bool skp_tryConnect();

#endif