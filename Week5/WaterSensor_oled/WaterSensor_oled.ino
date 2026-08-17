/*
 * ============================================
 *  Rain Water Sensor + LCD1602 I2C
 *  Hỗ trợ ESP8266 và ESP32
 * ============================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*=============================
    Rain Water Sensor
=============================*/
#if defined(ESP8266)
    #define DO_PIN D5
#elif defined(ESP32)
    #define DO_PIN 13
#endif

/*=============================
      LCD I2C
=============================*/
#if defined(ESP8266)
    #define SDA_PIN D2
    #define SCL_PIN D1
#elif defined(ESP32)
    #define SDA_PIN 19
    #define SCL_PIN 23
#endif

#define LCD_COLS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

/*=============================
    Prototype
=============================*/
void initSensor();
bool readDO();

void initLcd();
void printLcd(String line1, String line2);

/*=============================
    Setup
=============================*/
void setup()
{
    Serial.begin(115200);

    initSensor();
    initLcd();
}

/*=============================
    Loop
=============================*/
void loop()
{
    bool rain = readDO();

    if (rain)
    {
        Serial.println("Rain Detected");

        printLcd("Rain Sensor", "Rain: YES");
    }
    else
    {
        Serial.println("No Rain");

        printLcd("Rain Sensor", "Rain: NO");
    }

    delay(1000);
}

/*=============================
    Rain Sensor
=============================*/

// Khởi tạo cảm biến
void initSensor()
{
    pinMode(DO_PIN, INPUT);
}

// Đọc tín hiệu Digital
bool readDO()
{
    // LOW = Có mưa
    if (digitalRead(DO_PIN) == LOW)
        return true;
    else
        return false;
}

/*=============================
      LCD Functions
=============================*/

void initLcd()
{
    Wire.begin(SDA_PIN, SCL_PIN);

    lcd.init();
    lcd.backlight();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Rain Sensor");

    lcd.setCursor(0, 1);
    lcd.print("System Ready");

    delay(2000);

    lcd.clear();
}

void printLcd(String line1, String line2)
{
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(line1);

    lcd.setCursor(0, 1);
    lcd.print(line2);
}