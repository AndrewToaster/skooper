#include "SkpCommon.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "SkpUtil.h"
#include "SkpGist.h"
#include <Arduino.h>

static AsyncWebServer server(80);
static skp_data sensors[3];
static HTTPClient http;

#define __parseArg(req, name, val, method, defaultValue) ((req)->hasArg((name)) && (method)((req)->arg((name)), &val) ? (val) : defaultValue)
#define __parseArgF(req, name, val) __parseArg(req, name, val, skp_str2float, NAN)
//#define __parseArgUI(req, name, val) __parseArg(req, name, val, skp_str2uint, -1)

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
    if (!SPIFFS.begin())
    {
        skp_log_error("Failed to init SPIFFS");
        skp_halt();
    }

    server.serveStatic("/", SPIFFS, "/")
        .setDefaultFile("index.html");

    server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *req) {
        skp_log_info("UPDATE Called");
        if (!req->hasArg("index"))
        {
            req->send(400, MIME_PLAIN, "Missing index param");
            return;
        }

        int index;
        if (!skp_str2uint(req->arg("index"), &index))
        {
            req->send(400, MIME_PLAIN, "Invalid index param");
        }

        // We **REALLY** don't want to have buffer overflows in C++
        if (index < 0 || index > skp_arr_size(sensors, skp_data) - 1)
        {
            req->send(404, MIME_PLAIN, "Not found with index param");
            return;
        }

        float temp, hum, pres;
        sensors[index].temp = __parseArgF(req, "temp", temp);
        sensors[index].hum  = __parseArgF(req, "hum",  hum);
        sensors[index].pres = __parseArgF(req, "pres", pres);
        skp_logf_info("%i\n%f °C\n%f %%\n%f hPa\n", index, temp, hum, pres);

        req->send(200, MIME_PLAIN, emptyString);
    });

    server.on("/get", [] (AsyncWebServerRequest *req) {
        skp_log_info("GET Called");

        if (!req->hasArg("index"))
        {
            req->send(400, MIME_PLAIN, "Missing index param");
            return;
        }
        
        int index;
        if (!skp_str2uint(req->arg("index"), &index))
        {
            req->send(400, MIME_PLAIN, "Invalid index param");
        }

        // We **REALLY** don't want to have buffer overflows in C++
        if (index < 0 || index > skp_arr_size(sensors, skp_data) - 1)
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
    skp_log_debug("Updating IPFile");
    skp_gist_updateIPFile(WiFi.localIP().toString());
    delay(1000 * 60 * 5);
}