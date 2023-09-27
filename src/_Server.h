#include "SkpCommon.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "SkpUtil.h"
#include <Arduino.h>

static AsyncWebServer server(80);

static skp_data sensors[3];

void setup()
{
    Serial.println("Running as Server");
    
    for (size_t i = 0; i < sizeof(sensors) / sizeof(skp_data); i++)
    {
        sensors[i] = (skp_data) {
            .temp = NAN,
            .hum = NAN,
            .pres = NAN
        };
    }
    

    //File index, sensor;

    if (!SPIFFS.begin() )//|| !(index = SPIFFS.open("/index.html")) || !(index = SPIFFS.open("/sensor.html")))
    {
        skp_log_error("Failed to init SPIFFS");
        skp_halt();
    }

    //index_html = index.readString();
    //sensor_html = sensor.readString();

    //Serial.println(index_html.length());
    //Serial.println(sensor_html.length());

    server.serveStatic("/", SPIFFS, "/")
        .setDefaultFile("index.html");

    server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *req) {
        if (!req->hasArg("index"))
        {
            req->send(400, MIME_PLAIN, "Missing index param");
            return;
        }

        // We **REALLY** don't want to have buffer overflows in C++
        auto indexArg = req->arg("index");
        int index = indexArg.toInt();
        if (index < 0 || index > sizeof(sensors) / sizeof(skp_data) - 1)
        {
            req->send(404, MIME_PLAIN, "Not found with index param");
            return;
        }

        // There should be some proper parsing of the numbers, however for now this is ok
        auto tempArg = req->arg("temp");
        float temp = tempArg.toFloat();

        //TODO: Most likely humidity of 0% is not possible, therefore there could be error handling using that?
        auto humArg = req->arg("hum");
        float hum = humArg.toFloat();

        auto presArg = req->arg("pres");
        float pres = humArg.toFloat();

        sensors[index].temp = temp;
        sensors[index].hum = hum;
        sensors[index].pres = pres;

        req->send(200, MIME_PLAIN, emptyString);
    });

    server.on("/get", [] (AsyncWebServerRequest *req) {
        if (!req->hasArg("index"))
        {
            req->send(400, MIME_PLAIN, "Missing index param");
            return;
        }

        // We **REALLY** don't want to have buffer overflows in C++
        auto indexArg = req->arg("index");
        int index = indexArg.toInt();
        if (index < 0 || index > sizeof(sensors) / sizeof(skp_data) - 1)
        {
            req->send(404, MIME_PLAIN, "Not found with index param");
            return;
        }

        req->send(200, MIME_JSON, skp_serializeData(&sensors[index]));
    });

    server.onNotFound([] (AsyncWebServerRequest *req) {
        req->send(404, "text/plain", "Did not find");
    });

    server.begin();
}

void loop()
{
    delay(100);
}