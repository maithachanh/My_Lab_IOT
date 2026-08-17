/*
 * ============================================
 *  Sound Sensor + LCD1602 I2C
 *  Hỗ trợ ESP8266 và ESP32
 * ============================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*=====================================
      Sound Sensor
=====================================*/
#if defined(ESP8266)
    #define DO_PIN D5
#elif defined(ESP32)
    #define DO_PIN 13
#endif

/*=====================================
      LCD I2C
=====================================*/
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

/*=====================================
      Prototype
=====================================*/
void initSensor();
void readDO();

void initLcd();
void printLcd(String line1, String line2);

/*=====================================
      Biến toàn cục
=====================================*/
int clap = 0;
unsigned long detection_range_start = 0;
unsigned long detection_range = 0;

bool lastState = HIGH;

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
    readDO();
}

/*=====================================
      Sound Sensor
=====================================*/

void initSensor()
{
    pinMode(DO_PIN, INPUT);
}

void readDO()
{
    int dValue = digitalRead(DO_PIN);

    // Chỉ đếm khi chuyển từ HIGH -> LOW
    if (lastState == HIGH && dValue == LOW)
    {
        if (clap == 0)
        {
            clap = 1;
            detection_range_start = millis();
            detection_range = millis();
        }
        else
        {
            if (millis() - detection_range >= 100)
            {
                clap++;
                detection_range = millis();
            }
        }
    }

    lastState = dValue;

    // Sau 300ms thì kiểm tra số lần vỗ tay
    if (clap > 0 && (millis() - detection_range_start >= 300))
    {
        if (clap == 1)
        {
            Serial.println("CLAP 1 time");
            printLcd("CLAP", "1 time");
        }
        else if (clap == 2)
        {
            Serial.println("CLAP 2 times");
            printLcd("CLAP", "2 times");
        }
        else
        {
            Serial.print("CLAP ");
            Serial.print(clap);
            Serial.println(" times");

            printLcd("CLAP", String(clap) + " times");
        }

        clap = 0;
    }
}

/*=====================================
      LCD Functions
=====================================*/

void initLcd()
{
    Wire.begin(SDA_PIN, SCL_PIN);

    lcd.init();
    lcd.backlight();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Sound Sensor");

    lcd.setCursor(0, 1);
    lcd.print("System Ready");

    delay(2000);

    lcd.clear();

    printLcd("Sound Sensor", "Waiting...");
}

void printLcd(String line1, String line2)
{
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(line1);

    lcd.setCursor(0, 1);
    lcd.print(line2);
}