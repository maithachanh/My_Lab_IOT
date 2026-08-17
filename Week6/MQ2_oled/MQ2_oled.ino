/*
 * =====================================================
 *  MQ2 Gas Sensor + LCD1602 I2C
 *  ESP8266 / ESP32
 * =====================================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*=====================================================
    MQ2 Sensor
=====================================================*/
#if defined(ESP8266)

    #define DO_PIN D5

#elif defined(ESP32)

    #define DO_PIN 13

#endif

/*=====================================================
    LCD I2C
=====================================================*/
#if defined(ESP8266)

    #define SDA_PIN D2
    #define SCL_PIN D1

#elif defined(ESP32)

    #define SDA_PIN 19
    #define SCL_PIN 23

#endif

#define LCD_ADDRESS 0x27
#define LCD_COLS    16
#define LCD_ROWS    2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

/*=====================================================
    Function Prototype
=====================================================*/

void initSensor();
bool readMQ2();

void initLCD();
void printLcd(String line1, String line2);

/*=====================================================
    Setup
=====================================================*/

void setup()
{
    Serial.begin(115200);

    Wire.begin(SDA_PIN, SCL_PIN);

    initSensor();
    initLCD();

    Serial.println();
    Serial.println("==============================");
    Serial.println(" MQ2 Gas Monitoring");
    Serial.println("==============================");
}

/*=====================================================
    Loop
=====================================================*/

void loop()
{
    bool smoke = readMQ2();

    if (smoke)
    {
        Serial.println("Smoke: DETECTED");
        printLcd("MQ2 Sensor", "Smoke: +");
    }
    else
    {
        Serial.println("Smoke: SAFE");
        printLcd("MQ2 Sensor", "Smoke: -");
    }

    delay(1000);
}

/*=====================================================
    MQ2 Sensor
=====================================================*/

void initSensor()
{
    pinMode(DO_PIN, INPUT);
}

bool readMQ2()
{
    int dValue = digitalRead(DO_PIN);

    // LOW  -> Smoke detected
    // HIGH -> No smoke

    return (dValue == LOW);
}

/*=====================================================
    LCD Initialization
=====================================================*/

void initLCD()
{
    lcd.init();
    lcd.backlight();

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("MQ2 Monitor");

    lcd.setCursor(0, 1);
    lcd.print("Initializing");

    delay(2000);

    lcd.clear();
}

/*=====================================================
    Print LCD
=====================================================*/

void printLcd(String line1, String line2)
{
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(line1);

    lcd.setCursor(0, 1);
    lcd.print(line2);
}