#include "SkpWifiCommon.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "SkpUtil.h"

#define SSID "ISSWF1"
#define PASS "xstudent25isswf100"
#define RETRY_COUNT 15

static String emptyStr = String();

bool skp_tryConnect()
{
    WiFi.disconnect(false, false);
    WiFi.setAutoReconnect(true);
    WiFi.mode(WIFI_STA);
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
    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());
    return true;
}


String skp_serializeData(const skp_data *data)
{
    // Oops, null pointer
    if (!data) 
    {
        skp_log_error("data is null pointer");
        return emptyString;
    }

    // This is a very lazy way to do this, but it's fine for now
    String payload = "{ \"temperature\": ";
    payload += data->temp;
    //payload += ", \"pressure\": ";
    //payload += data->pres;
    payload += ", \"humidity\": ";
    payload += data->hum;
    payload += "}";

    return payload;  
}