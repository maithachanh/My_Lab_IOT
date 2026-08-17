/*
 * ============================================
 *  Flame Sensor + LCD1602 I2C
 *  Hỗ trợ ESP8266 và ESP32
 * ============================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*=====================================
    Khai báo chân cảm biến lửa
=====================================*/
#if defined(ESP8266)
    #define DO_PIN D5
#elif defined(ESP32)
    #define DO_PIN 13
#endif

/*=====================================
    Khai báo chân I2C cho LCD
=====================================*/
#if defined(ESP8266)
    #define SDA_PIN D2
    #define SCL_PIN D1
#elif defined(ESP32)
    #define SDA_PIN 19
    #define SCL_PIN 23
#endif

/*=====================================
    Khởi tạo LCD1602 I2C
=====================================*/
#define LCD_COLS 16
#define LCD_ROWS 2

// Địa chỉ I2C của LCD (thường là 0x27 hoặc 0x3F)
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

/*=====================================
    Khai báo Prototype
=====================================*/
void initSensor();
bool readDO();

void initLcd();
void printLcd(String line1, String line2);

/*=====================================
    Setup
=====================================*/
void setup()
{
    Serial.begin(115200);

    initSensor();
    initLcd();
}

/*=====================================
    Loop
=====================================*/
void loop()
{
    bool flame = readDO();

    if (flame)
    {
        Serial.println("Flame: Detected");
        printLcd("Flame Sensor", "Flame: +");
    }
    else
    {
        Serial.println("Flame: Not Detected");
        printLcd("Flame Sensor", "Flame: -");
    }

    delay(1000);
}

/*=====================================
        Flame Sensor Functions
=====================================*/

// Khởi tạo cảm biến
void initSensor()
{
    pinMode(DO_PIN, INPUT);
}

// Đọc tín hiệu Digital
bool readDO()
{
    int dValue = digitalRead(DO_PIN);

    // LOW  : Có lửa
    // HIGH : Không có lửa
    return (dValue == LOW);
}

/*=====================================
        LCD Functions
=====================================*/

// Khởi tạo LCD
void initLcd()
{
#if defined(ESP8266)
    Wire.begin(SDA_PIN, SCL_PIN);
#elif defined(ESP32)
    Wire.begin(SDA_PIN, SCL_PIN);
#endif

    lcd.init();
    lcd.backlight();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Flame Sensor");
    lcd.setCursor(0, 1);
    lcd.print("System Ready");

    delay(2000);
    lcd.clear();
}

// Hiển thị dữ liệu lên LCD
void printLcd(String line1, String line2)
{
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(line1);

    lcd.setCursor(0, 1);
    lcd.print(line2);
}