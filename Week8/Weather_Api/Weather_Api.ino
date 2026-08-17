/*
 * =====================================================
 * Weather API - ESP32 / ESP8266
 * =====================================================
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif
    #include <ArduinoJson.h>

/*=====================================================
    WiFi Configuration
=====================================================*/
#define AP_SSID      "HSU_Students"
#define AP_PASSWORD  "tontrongsukhacbiet"

/*=====================================================
    Weather API Configuration
=====================================================*/
String API_KEY = "7ed0868040a74a2ea3821027262907";
String CITY    = "Ho_Chi_Minh";

/*=====================================================
    Function Prototype
=====================================================*/
void initWifi();
void reconnectWiFi();
void getWeather();

/*=====================================================
    Setup
=====================================================*/
void setup()
{
    Serial.begin(115200);
    delay(1000);

    initWifi();
}

/*=====================================================
    Loop
=====================================================*/
void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        reconnectWiFi();
    }

    getWeather();

    // Cập nhật mỗi 1 phút
    delay(60000);
}

/*=====================================================
    Connect WiFi
=====================================================*/
void initWifi()
{
    WiFi.mode(WIFI_STA);

#if defined(ESP32)
    WiFi.setSleep(false);      // Giữ WiFi ổn định trên ESP32
#endif

    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(AP_SSID);

    WiFi.begin(AP_SSID, AP_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("=================================");
    Serial.println("WiFi Connected");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());

    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    Serial.println("=================================");
}

/*=====================================================
    Reconnect WiFi
=====================================================*/
void reconnectWiFi()
{
    Serial.println("WiFi disconnected!");
    Serial.println("Reconnecting...");

    WiFi.disconnect();
    WiFi.begin(AP_SSID, AP_PASSWORD);

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Reconnected successfully.");
    }
    else
    {
        Serial.println("Reconnect failed.");
    }
}

/*=====================================================
    Get Weather
=====================================================*/
void getWeather()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi is not connected.");
        return;
    }

    // URL đúng của WeatherAPI
    String url =
        "http://api.weatherapi.com/v1/current.json?key=" +
        API_KEY +
        "&q=" +
        CITY +
        "&aqi=no";

    Serial.println();
    Serial.println("=================================");
    Serial.println("Request URL:");
    Serial.println(url);

    HTTPClient http;

#if defined(ESP8266)
    WiFiClient client;
    http.begin(client, url);
#elif defined(ESP32)
    WiFiClient client;
    http.begin(client, url);
#endif

    int httpCode = http.GET();

    Serial.print("HTTP Code: ");
    Serial.println(httpCode);

    if (httpCode > 0)
    {
        String payload = http.getString();

        if (httpCode == HTTP_CODE_OK)
        {
            Serial.println("Weather data received.");

            DynamicJsonDocument doc(4096);

            DeserializationError error = deserializeJson(doc, payload);

            if (error)
            {
                Serial.print("JSON Error: ");
                Serial.println(error.c_str());
            }
            else
            {
                String location =
                    doc["location"]["name"].as<String>();

                String country =
                    doc["location"]["country"].as<String>();

                float temp =
                    doc["current"]["temp_c"].as<float>();

                float feelsLike =
                    doc["current"]["feelslike_c"].as<float>();

                int humidity =
                    doc["current"]["humidity"].as<int>();

                float wind =
                    doc["current"]["wind_kph"].as<float>();

                float pressure =
                    doc["current"]["pressure_mb"].as<float>();

                String condition =
                    doc["current"]["condition"]["text"].as<String>();

                Serial.println("---------------------------------");
                Serial.print("Location     : ");
                Serial.print(location);
                Serial.print(", ");
                Serial.println(country);

                Serial.print("Temperature  : ");
                Serial.print(temp);
                Serial.println(" °C");

                Serial.print("Feels Like   : ");
                Serial.print(feelsLike);
                Serial.println(" °C");

                Serial.print("Humidity     : ");
                Serial.print(humidity);
                Serial.println(" %");

                Serial.print("Wind Speed   : ");
                Serial.print(wind);
                Serial.println(" km/h");

                Serial.print("Pressure     : ");
                Serial.print(pressure);
                Serial.println(" mb");

                Serial.print("Condition    : ");
                Serial.println(condition);

                Serial.println("---------------------------------");
            }
        }
        else
        {
            Serial.println("Server returned an error:");
            Serial.println(payload);
        }
    }
    else
    {
        Serial.print("GET Failed: ");
        Serial.println(http.errorToString(httpCode));
    }

    http.end();

    Serial.println("=================================");
}