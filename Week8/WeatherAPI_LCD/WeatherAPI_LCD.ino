/*
 * =====================================================
 * Weather API + LCD I2C - ESP32 / ESP8266
 * =====================================================
 *
 * Đấu dây I2C (theo yêu cầu, mặc định cho ESP32):
 *   SCL -> GPIO19  (IO19)
 *   SDA -> GPIO23  (IO23)
 *   GND -> GND
 *   5V  -> 5V (VCC của module LCD I2C)
 *
 * Thư viện cần cài thêm (Sketch > Include Library > Manage Libraries):
 *   - "LiquidCrystal I2C" (tác giả: Frank de Brabander)
 *   - ArduinoJson (đã dùng từ code gốc)
 *
 * Ghi chú: LCD 16x2 không hiển thị được dấu tiếng Việt (không dấu),
 * nên toàn bộ chuỗi hiển thị lên LCD được viết không dấu.
 * =====================================================
 */

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif

#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

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
    I2C / LCD Configuration
=====================================================*/
#if defined(ESP32)
  #define I2C_SDA_PIN   23   // theo yêu cầu: SDA -> IO23
  #define I2C_SCL_PIN   19   // theo yêu cầu: SCL -> IO19
#elif defined(ESP8266)
  // ESP8266 không có chân GPIO19/23, dùng chân I2C mặc định D2/D1.
  // Nếu dùng ESP8266, hãy đấu lại dây theo 2 chân này.
  #define I2C_SDA_PIN   4    // D2
  #define I2C_SCL_PIN   5    // D1
#endif

#define LCD_COLS      16
#define LCD_ROWS      2

LiquidCrystal_I2C* lcd = nullptr;
uint8_t lcdAddress = 0x27;    // sẽ được dò lại tự động trong setup()

/*=====================================================
    Thời gian (non-blocking, không dùng delay() dài)
=====================================================*/
const unsigned long WEATHER_INTERVAL = 60000;   // 60 giây: gọi API 1 lần
const unsigned long DISPLAY_INTERVAL = 3000;    // 3 giây: đổi trang màn hình

unsigned long lastWeatherFetch  = 0;
unsigned long lastDisplaySwitch = 0;
uint8_t       displayPage       = 0;
const uint8_t TOTAL_PAGES       = 4;

/*=====================================================
    Dữ liệu thời tiết (lưu toàn cục để LCD dùng lại)
=====================================================*/
bool   weatherAvailable = false;
String g_location  = "";
String g_country   = "";
float  g_temp       = 0;
float  g_feelsLike  = 0;
int    g_humidity   = 0;
float  g_wind       = 0;
float  g_pressure   = 0;
String g_condition  = "";

/*=====================================================
    Function Prototype
=====================================================*/
void initWifi();
void reconnectWiFi();
void getWeather();
void initLCD();
uint8_t scanI2CAddress();
void lcdMessage(const String &line1, const String &line2 = "");
void updateDisplay();

/*=====================================================
    Setup
=====================================================*/
void setup()
{
    Serial.begin(115200);
    delay(1000);

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);   // SDA, SCL
    initLCD();

    lcdMessage("Dang ket noi", "WiFi...");
    initWifi();

    lcdMessage("WiFi da ket noi", WiFi.localIP().toString());
    delay(1500);
}

/*=====================================================
    Loop
=====================================================*/
void loop()
{
    unsigned long now = millis();

    if (WiFi.status() != WL_CONNECTED)
    {
        lcdMessage("Mat ket noi WiFi", "Dang ket noi lai");
        reconnectWiFi();
    }

    // Gọi API thời tiết mỗi WEATHER_INTERVAL (và ngay lần đầu tiên)
    if (lastWeatherFetch == 0 || now - lastWeatherFetch >= WEATHER_INTERVAL)
    {
        lastWeatherFetch = now;
        getWeather();
    }

    // Đổi trang hiển thị trên LCD mỗi DISPLAY_INTERVAL
    if (now - lastDisplaySwitch >= DISPLAY_INTERVAL)
    {
        lastDisplaySwitch = now;
        displayPage = (displayPage + 1) % TOTAL_PAGES;
        updateDisplay();
    }
}

/*=====================================================
    Khởi tạo LCD I2C (tự dò địa chỉ I2C)
=====================================================*/
void initLCD()
{
    lcdAddress = scanI2CAddress();

    lcd = new LiquidCrystal_I2C(lcdAddress, LCD_COLS, LCD_ROWS);
    lcd->init();
    lcd->backlight();
    lcd->clear();
}

/*=====================================================
    Dò địa chỉ I2C của LCD
    (địa chỉ thường gặp: 0x27 hoặc 0x3F tùy loại module)
=====================================================*/
uint8_t scanI2CAddress()
{
    Serial.println("Dang do dia chi I2C...");

    for (uint8_t addr = 1; addr < 127; addr++)
    {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0)
        {
            Serial.print("Tim thay thiet bi I2C tai dia chi: 0x");
            Serial.println(addr, HEX);
            return addr;
        }
    }

    Serial.println("Khong tim thay thiet bi I2C nao! Dung dia chi mac dinh 0x27.");
    return 0x27;
}

/*=====================================================
    Hiển thị 1-2 dòng lên LCD (tự cắt bớt nếu quá dài)
=====================================================*/
void lcdMessage(const String &line1, const String &line2)
{
    if (!lcd) return;

    lcd->clear();

    lcd->setCursor(0, 0);
    lcd->print(line1.substring(0, LCD_COLS));

    if (line2.length() > 0)
    {
        lcd->setCursor(0, 1);
        lcd->print(line2.substring(0, LCD_COLS));
    }
}

/*=====================================================
    Cập nhật LCD theo trang hiện tại (xoay vòng thông tin)
=====================================================*/
void updateDisplay()
{
    if (!weatherAvailable)
    {
        lcdMessage("Dang cho du lieu", "thoi tiet...");
        return;
    }

    switch (displayPage)
    {
        case 0:
        {
            String line1 = g_location + ", " + g_country;
            String line2 = "Temperature: " + String(g_temp, 1) + "C";
            lcdMessage(line1, line2);
            break;
        }
        case 1:
        {
            String line1 = "Feels Like: " + String(g_feelsLike, 1) + "C";
            String line2 = "Humidity: " + String(g_humidity) + "%";
            lcdMessage(line1, line2);
            break;
        }
        case 2:
        {
            String line1 = "Wind Speed: " + String(g_wind, 1) + "km/h";
            String line2 = "Condition: " + String(g_pressure, 0) + "mb";
            lcdMessage(line1, line2);
            break;
        }
        case 3:
        {
            String line1 = "Thoi tiet:";
            String line2 = g_condition;
            lcdMessage(line1, line2);
            break;
        }
    }
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
    WiFiClient client;
    http.begin(client, url);

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
                lcdMessage("Loi doc JSON");
            }
            else
            {
                g_location  = doc["location"]["name"].as<String>();
                g_country   = doc["location"]["country"].as<String>();
                g_temp      = doc["current"]["temp_c"].as<float>();
                g_feelsLike = doc["current"]["feelslike_c"].as<float>();
                g_humidity  = doc["current"]["humidity"].as<int>();
                g_wind      = doc["current"]["wind_kph"].as<float>();
                g_pressure  = doc["current"]["pressure_mb"].as<float>();
                g_condition = doc["current"]["condition"]["text"].as<String>();

                weatherAvailable = true;

                Serial.println("---------------------------------");
                Serial.print("Location     : ");
                Serial.print(g_location);
                Serial.print(", ");
                Serial.println(g_country);

                Serial.print("Temperature  : ");
                Serial.print(g_temp);
                Serial.println(" °C");

                Serial.print("Feels Like   : ");
                Serial.print(g_feelsLike);
                Serial.println(" °C");

                Serial.print("Humidity     : ");
                Serial.print(g_humidity);
                Serial.println(" %");

                Serial.print("Wind Speed   : ");
                Serial.print(g_wind);
                Serial.println(" km/h");

                Serial.print("Pressure     : ");
                Serial.print(g_pressure);
                Serial.println(" mb");

                Serial.print("Condition    : ");
                Serial.println(g_condition);
                Serial.println("---------------------------------");

                // Có dữ liệu mới -> quay lại trang đầu và hiển thị ngay
                displayPage = 0;
                lastDisplaySwitch = millis();
                updateDisplay();
            }
        }
        else
        {
            Serial.println("Server returned an error:");
            Serial.println(payload);
            lcdMessage("Loi may chu", "HTTP " + String(httpCode));
        }
    }
    else
    {
        Serial.print("GET Failed: ");
        Serial.println(http.errorToString(httpCode));
        lcdMessage("Loi ket noi API");
    }

    http.end();

    Serial.println("=================================");
}
