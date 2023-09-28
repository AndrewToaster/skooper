#include "SkpCommon.h"
#include <Arduino.h>
#include <WiFi.h>
#include "SkpUtil.h"

#define RETRY_COUNT 15

const char *MIME_HTML = "text/html";
const char *MIME_PLAIN = "text/plain";
const char *MIME_JSON = "application/json";

bool skp_tryConnect()
{
    WiFi.disconnect(false, false);
    WiFi.setAutoReconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WF_SSID, WF_PASS);
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
        count++;
        delay(1000);
    }
    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());
    return true;
}

void skp_initSerial()
{
    Serial.begin(115200);
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
    payload += ", \"pressure\": ";
    payload += data->pres;
    payload += ", \"humidity\": ";
    payload += data->hum;
    payload += "}";

    return payload;  
}