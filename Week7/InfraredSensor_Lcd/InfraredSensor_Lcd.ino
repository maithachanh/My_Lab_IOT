/*
 * =====================================================
 *  Infrared obstacle sensor + LCD1602 I2C
 *  ESP8266 / ESP32
 * =====================================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*=====================================================
    Infrared obstacle sensor
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
bool readDO();

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
    Serial.println(" Infrared Monitoring");
    Serial.println("==============================");
}

// loop to run repeatedly
void loop() {

    bool obstacle = readDO();

    if (obstacle) {
        //Serial.println("Obstacle: +");
        printLcd("Infrared sensor", "Obstacle: +");
    } else {
        //Serial.println("Obstacle: -");
        printLcd("Infrared sensor", "Obstacle: -");
    }

    delay(1000);
}

/* Infrared obstacle sensor */
void initSensor() {
    pinMode(DO_PIN, INPUT);
}

bool readDO() {
    int dValue = digitalRead(DO_PIN);

    if(dValue)
      return false;
    else
      return true;
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
    lcd.print("Infrared Monitor");

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