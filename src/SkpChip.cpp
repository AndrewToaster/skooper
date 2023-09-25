#include "SkpChip.h"
#include <WString.h>
#include <DHT.h>
#include <HTTPClient.h>
#include "SkpUtil.h"

#define DHT_PIN 13

static auto sensor = DHT(DHT11, DHT_PIN); 
static HTTPClient client;

bool skp_sendData(const skp_data *data)
{
    // This is a very lazy way to do this, but it's fine for now
    String payload = "{ \"temperature\": ";
    payload += data->temp;
    //payload += ", \"pressure\": ";
    //payload += data->pres;
    payload += ", \"humidity\": ";
    payload += data->hum;
    payload += "}";

    

    return true;   
}

bool skp_readData(skp_data *data)
{
    // Oops, null pointer
    if (!data) 
    {
        skp_log_error("data is null pointer");
        return false;
    }

    // Read the data
    data->hum = sensor.readHumidity();
    data->temp = sensor.readTemperature();

    return true;
}