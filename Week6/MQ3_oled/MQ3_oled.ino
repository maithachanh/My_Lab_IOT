/*
 * =====================================================
 *  MQ3 Alcohol Sensor + LCD1602 I2C
 *  ESP8266 / ESP32
 * =====================================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*=====================================================
    MQ3 Sensor
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
bool readMQ3();

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
    Serial.println(" MQ3 Alcohol Monitoring");
    Serial.println("==============================");
}

/*=====================================================
    Loop
=====================================================*/

void loop()
{
    bool alcohol = readMQ3();

    if (alcohol)
    {
        Serial.println("Alcohol: DETECTED");
        printLcd("MQ3 Sensor", "Alcohol: +");
    }
    else
    {
        Serial.println("Alcohol: SAFE");
        printLcd("MQ3 Sensor", "Alcohol: -");
    }

    delay(1000);
}

/*=====================================================
    MQ3 Sensor
=====================================================*/

void initSensor()
{
    pinMode(DO_PIN, INPUT);
}

bool readMQ3()
{
    int dValue = digitalRead(DO_PIN);

    // LOW  -> Alcohol detected
    // HIGH -> No alcohol

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
    lcd.print("MQ3 Monitor");

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