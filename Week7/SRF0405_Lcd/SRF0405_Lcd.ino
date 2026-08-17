/*
 * =====================================================
 *  Ultrasonic Sensor (SRF04 / SRF05) + LCD1602 I2C
 *  ESP8266 / ESP32
 * =====================================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*=====================================================
    SRF04 / SRF05 Pins
=====================================================*/
#if defined(ESP8266)

#define TRIG_PIN D5
#define ECHO_PIN D6

#elif defined(ESP32)

#define TRIG_PIN 13
#define ECHO_PIN 12

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
#define LCD_COLS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

/*=====================================================
    Function Prototype
=====================================================*/
void initSensor();
float readSRF05();

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
    Serial.println("      SRF05 Monitoring");
    Serial.println("==============================");
}

/*=====================================================
    Loop
=====================================================*/
void loop()
{
    float distance = readSRF05();

    if (distance < 0)
    {
        Serial.println("No Object");

        printLcd("SRF05", "No Object");
    }
    else
    {
        Serial.print("Distance: ");
        Serial.print(distance, 1);
        Serial.println(" cm");

        printLcd("SRF05", String(distance, 1) + " cm");
    }

    delay(1000);
}

/*=====================================================
    Sensor Initialization
=====================================================*/
void initSensor()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    digitalWrite(TRIG_PIN, LOW);
}

/*=====================================================
    Read Distance (cm)
=====================================================*/
float readSRF05()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duration == 0)
    {
        return -1;
    }

    float distance = duration * 0.0343 / 2.0;

    return distance;
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
    lcd.print("SRF05 Monitor");

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