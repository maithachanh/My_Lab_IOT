/*
 * =====================================================
 *  RFID MFRC522 + LCD1602 I2C
 *  ESP8266 / ESP32
 * =====================================================
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*=====================================================
    MFRC522 Pin
=====================================================*/
#if defined(ESP8266)

#define SS_PIN   D3
#define RST_PIN  D4

#elif defined(ESP32)

#define SS_PIN    5
#define SCK_PIN   18
#define MISO_PIN  19
#define MOSI_PIN  23
#define RST_PIN   13

#endif

/*=====================================================
    MFRC522 Object
=====================================================*/
MFRC522 mfrc522(SS_PIN, RST_PIN);

/*=====================================================
    Function Prototype
=====================================================*/
void initMFRC522();

String getCardUID();
String getCardUIDinHEX(byte *buffer, byte bufferSize);
String getCardUIDinDEC(byte *buffer, byte bufferSize);

/*=====================================================
    LCD I2C
=====================================================*/
#if defined(ESP8266)

#define SDA_PIN D2
#define SCL_PIN D1

#elif defined(ESP32)

#define SDA_PIN 27
#define SCL_PIN 16

#endif

#define LCD_ADDRESS 0x27
#define LCD_COLS    16
#define LCD_ROWS    2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

/*=====================================================
    LCD Prototype
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

    initMFRC522();
    initLCD();

    Serial.println();
    Serial.println("==========================");
    Serial.println(" RFID MFRC522 Monitoring");
    Serial.println("==========================");
}

/*=====================================================
    Loop
=====================================================*/
void loop()
{
    String uid = getCardUID();

    if (uid == "")
    {
        delay(100);
        return;
    }

    Serial.print("Card UID = ");
    Serial.println(uid);

    //=========================
    // Thay UID thật của bạn
    //=========================

    if (uid == "224-45-214-11")
    {
        printLcd("MFRC522", "White Card");
    }
    else if (uid == "79-126-152-189")
    {
        printLcd("MFRC522", "Blue Card");
    }
    else
    {
        printLcd("Unknown Card", uid);
    }

    delay(1000);
}

/*=====================================================
    MFRC522 Initialization
=====================================================*/
void initMFRC522()
{
#if defined(ESP32)
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
#else
    SPI.begin();
#endif

    mfrc522.PCD_Init();

    Serial.println("MFRC522 Initialized");
}

/*=====================================================
    Read Card UID
=====================================================*/
String getCardUID()
{
    if (!mfrc522.PICC_IsNewCardPresent())
        return "";

    if (!mfrc522.PICC_ReadCardSerial())
        return "";

    Serial.print("UID HEX : ");
    Serial.println(getCardUIDinHEX(
        mfrc522.uid.uidByte,
        mfrc522.uid.size));

    String uid = getCardUIDinDEC(
        mfrc522.uid.uidByte,
        mfrc522.uid.size);

    Serial.print("UID DEC : ");
    Serial.println(uid);

    Serial.println("--------------------------");

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();

    return uid;
}

/*=====================================================
    UID HEX
=====================================================*/
String getCardUIDinHEX(byte *buffer, byte bufferSize)
{
    String result = "";

    for (byte i = 0; i < bufferSize; i++)
    {
        if (buffer[i] < 0x10)
            result += "0";

        result += String(buffer[i], HEX);

        if (i < bufferSize - 1)
            result += "-";
    }

    result.toUpperCase();

    return result;
}

/*=====================================================
    UID DEC
=====================================================*/
String getCardUIDinDEC(byte *buffer, byte bufferSize)
{
    String result = "";

    for (byte i = 0; i < bufferSize; i++)
    {
        result += String(buffer[i]);

        if (i < bufferSize - 1)
            result += "-";
    }

    return result;
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
    lcd.print("MFRC522 Reader");

    lcd.setCursor(0, 1);
    lcd.print("Initializing");

    delay(2000);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Ready...");
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