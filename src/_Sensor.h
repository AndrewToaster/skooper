#include "SkpChip.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <AsyncUDP.h>
#include "SkpUtil.h"

#ifndef SENSOR_ID
#define SENSOR_ID 0
#warning SENSOR_ID not defined, assuming 0
#endif

static HTTPClient http;
static AsyncUDP udp;

static String endpoint = emptyString;

void setup()
{
    Serial.println("Running as Sensor");

    if (!udp.listenMulticast(IPAddress(239,255,5,5), 8001))
    {
        skp_log_error("Nobody is here :(");
        skp_halt();
    }
    
    udp.onPacket([] (AsyncUDPPacket& packet) {
        if (!packet.isBroadcast())
        {
            return;
        }

        Serial.println("OMG HAIIIII :3");
        endpoint = packet.readString();
    });
}

void loop()
{
    if (!endpoint.isEmpty())
    {
        Serial.println("FOund you!");
        Serial.println(endpoint);
        skp_halt();
    }
    delay(5000);
}