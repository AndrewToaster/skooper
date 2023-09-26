#include "SkpWifiCommon.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "SkpUtil.h"
#include <Arduino.h>

static AsyncWebServer server(80);

static skp_data sensors[3];

static const char *MIME_HTML = "text/html";
static const char *MIME_PLAIN = "text/plain";
static const char *MIME_JSON = "application/json";

void setup()
{
    sensors[0] = (skp_data) {
        .temp = 20,
        .hum = 50
    };

    Serial.begin(115200);
    while (!skp_tryConnect())
    {
        Serial.println("Retrying!");
        delay(5000);
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

    server.on("/update", HTTP_POST, [] (AsyncWebServerRequest *req) {
        if (!req->hasArg("index"))
        {
            req->send(400, MIME_PLAIN, "Missing index param");
            return;
        }

        auto indexArg = req->arg("index");
        int index = indexArg.toInt();
        if (index < 0 || index > sizeof(sensors) - 1)
        {
            req->send(404, MIME_PLAIN, "Not found with index param");
            return;
        }

        req->send(200, MIME_JSON, skp_serializeData(&sensors[index]));
    });

    server.on("/get", [] (AsyncWebServerRequest *req) {
        if (!req->hasArg("index"))
        {
            req->send(400, MIME_PLAIN, "Missing index param");
            return;
        }

        auto indexArg = req->arg("index");
        int index = indexArg.toInt();
        if (index < 0 || index > sizeof(sensors) - 1)
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