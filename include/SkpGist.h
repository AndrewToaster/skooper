#ifndef SKOOPER_SKPGIST_H
#define SKOOPER_SKPGIST_H

#include <WString.h>
#include <HTTPClient.h>

bool skp_gist_updateIPFile(const String& ip);
String skp_gist_readIPFile();

#endif