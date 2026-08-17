/*
 * =====================================================
 *  DHT11 + LCD1602 I2C
 *  ESP8266 / ESP32
 *  Compatible with DHT11 Library v2.1.0
 * =====================================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT11.h>

/*=====================================================
    DHT11
=====================================================*/
#if defined(ESP8266)

    #define DHT_PIN D5

#elif defined(ESP32)

    #define DHT_PIN 13

#endif

DHT11 dht11(DHT_PIN);

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

void initLCD();
bool readSensor(float &temperature, float &humidity);
void displayLCD(float temperature, float humidity);

/*=====================================================
    Setup
=====================================================*/

void setup()
{
    Serial.begin(115200);

    Wire.begin(SDA_PIN, SCL_PIN);

    initLCD();

    Serial.println();
    Serial.println("==================================");
    Serial.println(" DHT11 LCD Monitoring System");
    Serial.println("==================================");
}

/*=====================================================
    Loop
=====================================================*/

void loop()
{
    float temperature;
    float humidity;

    if (readSensor(temperature, humidity))
    {
        Serial.print("Temperature : ");
        Serial.print(temperature, 1);
        Serial.println(" °C");

        Serial.print("Humidity    : ");
        Serial.print(humidity, 1);
        Serial.println(" %");

        Serial.println("---------------------------");

        displayLCD(temperature, humidity);
    }
    else
    {
        Serial.println("Cannot read DHT11.");

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Sensor Error");
        lcd.setCursor(0,1);
        lcd.print("Check Wiring");
    }

    delay(2000);
}

/*=====================================================
    Initialize LCD
=====================================================*/

void initLCD()
{
    lcd.init();
    lcd.backlight();

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("DHT11 Monitor");

    lcd.setCursor(0,1);
    lcd.print("Initializing");

    delay(2000);

    lcd.clear();
}

/*=====================================================
    Read DHT11
=====================================================*/

bool readSensor(float &temperature, float &humidity)
{
    int temp;
    int humi;

    int result = dht11.readTemperatureHumidity(temp, humi);

    if(result == 0)
    {
        temperature = temp;
        humidity = humi;
        return true;
    }

    return false;
}

/*=====================================================
    Display LCD
=====================================================*/

void displayLCD(float temperature, float humidity)
{
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("Temp:");
    lcd.print(temperature,1);
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(0,1);
    lcd.print("Humi:");
    lcd.print(humidity,1);
    lcd.print("%");
}