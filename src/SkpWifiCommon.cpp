#include "SkpWifiCommon.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SSID ""
#define PASS ""
#define RETRY_COUNT 15
#define SERVER ""

bool skp_tryConnect()
{
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(SSID, PASS);
    Serial.print("Connecting to wifi ");
    int count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (count == RETRY_COUNT) 
        {
            Serial.print("Failed: ");
            Serial.println(WiFi.status());
            return false;
        }
        Serial.print(".");
        delay(1000);
    }
    
}