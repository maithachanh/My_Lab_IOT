/*
 * =====================================================
 *  Keypad 4x4 + LCD1602 I2C
 *  ESP8266 / ESP32
 * =====================================================
 */

#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*=====================================================
    Keypad Configuration
=====================================================*/

#define ROWS 4
#define COLS 4

#if defined(ESP8266)

byte rowPins[ROWS] = {D1, D2, D5, D6};
byte colPins[COLS] = {D7, D3, D4, D0};

#elif defined(ESP32)

byte rowPins[ROWS] = {27, 16, 17, 25};
byte colPins[COLS] = {5, 23, 19, 18};

#endif

char keys[ROWS][COLS] =
{
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

Keypad keypad = Keypad(
    makeKeymap(keys),
    rowPins,
    colPins,
    ROWS,
    COLS
);

/*=====================================================
    LCD1602 I2C
=====================================================*/

#if defined(ESP8266)

#define SDA_PIN D2
#define SCL_PIN D1

#elif defined(ESP32)

#define SDA_PIN 13
#define SCL_PIN 26

#endif

#define LCD_ADDRESS 0x27
#define LCD_COLS    16
#define LCD_ROWS    2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

/*=====================================================
    Function Prototype
=====================================================*/

void initLCD();
void printLcd(String line1, String line2);

/*=====================================================
    Setup
=====================================================*/

void setup()
{
    Serial.begin(115200);

    Wire.begin(SDA_PIN, SCL_PIN);

    initLCD();

    Serial.println();
    Serial.println("==============================");
    Serial.println(" Keypad Monitoring");
    Serial.println("==============================");

    printLcd("Keypad", "Ready");
}

/*=====================================================
    Loop
=====================================================*/

void loop()
{
    char key = keypad.getKey();

    if (key != NO_KEY)
    {
        Serial.print("Key Pressed : ");
        Serial.println(key);

        printLcd("Keypad", String(key));
    }

    delay(50);
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
    lcd.print("Keypad Reader");

    lcd.setCursor(0, 1);
    lcd.print("Initializing");

    delay(2000);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Keypad");

    lcd.setCursor(0, 1);
    lcd.print("Ready");
}

/*=====================================================
    Print LCD
=====================================================*/

void printLcd(String line1, String line2)
{
    // Xóa nội dung dòng 1
    lcd.setCursor(0, 0);
    lcd.print("                ");

    // Xóa nội dung dòng 2
    lcd.setCursor(0, 1);
    lcd.print("                ");

    // Hiển thị dòng 1
    lcd.setCursor(0, 0);
    lcd.print(line1);

    // Hiển thị dòng 2
    lcd.setCursor(0, 1);
    lcd.print(line2);
}