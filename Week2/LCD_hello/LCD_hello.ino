#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_COLS 16
#define LCD_ROWS 2

// Chọn chân SDA và SCL
#if defined(ESP8266)
    #define SDA_PIN D2
    #define SCL_PIN D1
#elif defined(ESP32)
    #define SDA_PIN 19
    #define SCL_PIN 23
#endif

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

void initLcd();
void printLcd(String line1, String line2);

void setup()
{
    initLcd();
}

void loop()
{
    printLcd("ThachAnh", "QuocDuy");
    delay(2000);
}

void initLcd()
{
    // Khởi tạo I2C với chân mong muốn
    Wire.begin(SDA_PIN, SCL_PIN);

    lcd.init();
    lcd.backlight();
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