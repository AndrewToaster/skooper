#include <Arduino.h>
#include <HTTPClient.h>
#include <AsyncUDP.h>
#include "SkpUtil.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "SkpCommon.h"
#include "SkpGist.h"

#ifndef SENSOR_ID
#define SENSOR_ID 0
#warning SENSOR_ID not defined, assuming 0
#endif

static Adafruit_BME280 bme;
static HTTPClient http;
//static AsyncUDP udp;

static String ip;

static uint next_check = 0;

// Built-in compile time string concatenation BABYYYYY 
//static String endpoint = "http:///update?index=" SKP_STRIFY(SENSOR_ID);

void setup()
{
    Serial.println("Running as Sensor");

    //: Such a shame that this wont work during showtime
    /*if (!udp.listenMulticast(IPAddress(239,255,5,5), 8001))
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

    //ip = skp_gist_readIPFile();

    Serial.println("Setup done");
}

void loop()
{
    if (next_check <= millis())
    {
        next_check = millis() + 60000;
        ip = skp_gist_readIPFile();
    }

    if (ip.isEmpty())
    {
        skp_log_error("Failed to get ip");
        delay(5000);
        return;
    }

    // Huh, cool. Don't have this in C#
    float temp = bme.readTemperature(),
          hum  = bme.readHumidity(),
          pres = bme.readPressure() / 100;


    skp_logf_info("temp - %f °C\nhum - %f %\npres - %f hPa\n", temp, hum, pres);
    
    String address = "http://" + ip + "/update"
        + "?index=" SKP_STRIFY(SENSOR_ID) 
        + "&temp="  + temp
        + "&hum="   + hum
        + "&pres="  + pres;
    
    skp_log_debug(address);

    if (!http.begin(address))
    {
        skp_log_error("Failed to parse url");
        return;
    }

    auto resp = http.GET();
    if (resp == 404)
    {
        skp_log_error(String("Got 404 error: ") + http.getString());
    }
    else if (resp < 0)
    {
        skp_log_error(String("Got internal error: ") + http.errorToString(resp));
    }
    else if (resp != 200)
    {
        skp_log_error(String("Unexpected response code ") + resp + ": " + http.getString());
    }
    else
    {
        Serial.println("Yipee, it worked");
    }
    http.end();

    delay(5000);
}