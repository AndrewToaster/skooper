#ifndef SERVER_CON_H
#define SERVER_CON_H

#include "SkpWifiCommon.h"

bool skp_sendData(const skp_data *data);
bool skp_readData(skp_data *data);

#endif