#include <Arduino.h>
#include <HTTPClient.h>
#include <AsyncUDP.h>
#include "SkpUtil.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#ifndef SENSOR_ID
#define SENSOR_ID 0
#warning SENSOR_ID not defined, assuming 0
#endif


Adafruit_BME280 bme; // I2C
static HTTPClient http;
static AsyncUDP udp;

static String endpoint = "http://192.168.215.96/update?index=0";

void setup()
{
    Serial.println("Running as Sensor");

    /*
    if (!udp.listenMulticast(IPAddress(239,255,5,5), 8001))
    {
        skp_log_error("Nobody is here :(");
        skp_halt();
    }
    
    udp.onPacket([] (AsyncUDPPacket& packet) {
        Serial.println("got packet!");
        if (!packet.isBroadcast())
        {
            return;
        }

        Serial.println("OMG HAIIIII :3");
        endpoint = packet.readString();
        Serial.print("Found server at ");
        Serial.println(endpoint);
    });*/
    
    if (!bme.begin(0x76))
    {
        skp_log_error("Failed to init sensor");
        skp_halt();
    }

    Serial.println("Setup done");
}

void loop()
{
    delay(5000);

    skp_data data = {
        .temp = bme.readTemperature(),
        .hum = bme.readHumidity(),
        .pres = bme.readPressure() / 100,
    };

    Serial.printf("%fC, %f%, %fhPa\n", data.temp, data.hum, data.pres);
    
    String address = endpoint
        + "&temp=" + String(data.temp)
        + "&hum=" + String(data.hum)
        + "&pres=" + String(data.pres);
    
    Serial.println(address);

    if (!http.begin(address))
    {
        skp_log_error("Failed to parse url");
        return;
    }
    
    auto resp = http.GET();
    if (resp == 404)
    {
        skp_log_errorS(String("Got 404 error: ") + http.getString());
    }
    else if (resp < 0)
    {
        skp_log_errorS(String("Got internal error: ") + http.errorToString(resp));
    }
    else if (resp != 200)
    {
        skp_log_errorS(String("Unexpected response code ") + resp + ": " + http.getString());
    }
    else
    {
        Serial.println("Yipee, it worked");
    }
    http.end();
}