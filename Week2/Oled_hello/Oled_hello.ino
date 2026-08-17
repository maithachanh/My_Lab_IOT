/*
 * OLED Hello Demo
 */

#if defined(ESP8266)
    #define SDA_PIN D2
    #define SCL_PIN D1
#elif defined(ESP32)
    #define SDA_PIN 23
    #define SCL_PIN 19
#endif

// Chọn loại màn hình OLED
#define ssd1306
// #define sh1106

#if defined(ssd1306)
    #include <SSD1306.h>     // https://github.com/ThingPulse/esp8266-oled-ssd1306
    SSD1306 oled(0x3C, SDA_PIN, SCL_PIN);
#elif defined(sh1106)
    #include <SH1106.h>      // https://github.com/ThingPulse/esp8266-oled-ssd1306
    SH1106 oled(0x3C, SDA_PIN, SCL_PIN);
#endif

// Prototype
void initOled();
void printOled(String line1, String line2);
void printOled(String line1, String line2, String line3);
void printOled(String line1, String line2, String line3, String line4);

void setup()
{
    initOled();
}

void loop()
{
    String line1 = "+ ThachAnh";
    String line2 = "+ 22206051";
    String line3 = "=> QuocDuy";
    String line4 = ">> 22206826";

    printOled(line1, line2);
    delay(2000);

    printOled(line1, line2, line3);
    delay(2000);

    printOled(line1, line2, line3, line4);
    delay(2000);
}

/*=========================
        OLED FUNCTIONS
=========================*/

void initOled()
{
    oled.init();
    oled.flipScreenVertically();
}

// Hiển thị 2 dòng
void printOled(String line1, String line2)
{
    oled.clear();

    oled.setTextAlignment(TEXT_ALIGN_LEFT);
    oled.setFont(ArialMT_Plain_24);

    oled.drawString(0, 0, line1);
    oled.drawString(0, 36, line2);

    oled.display();
}

// Hiển thị 3 dòng
void printOled(String line1, String line2, String line3)
{
    oled.clear();

    oled.setTextAlignment(TEXT_ALIGN_LEFT);
    oled.setFont(ArialMT_Plain_16);

    oled.drawString(0, 0, line1);
    oled.drawString(0, 24, line2);
    oled.drawString(0, 48, line3);

    oled.display();
}

// Hiển thị 4 dòng
void printOled(String line1, String line2, String line3, String line4)
{
    oled.clear();

    oled.setTextAlignment(TEXT_ALIGN_LEFT);
    oled.setFont(ArialMT_Plain_10);

    oled.drawString(0, 0, line1);
    oled.drawString(0, 16, line2);
    oled.drawString(0, 32, line3);
    oled.drawString(0, 48, line4);

    oled.display();
}