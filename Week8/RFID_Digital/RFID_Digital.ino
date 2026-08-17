/*
 * =====================================================
 *  RFID MFRC522 Reader
 *  ESP8266 / ESP32
 * =====================================================
 */

#include <SPI.h>
#include <MFRC522.h>

/*=====================================================
    MFRC522 Pin Configuration
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
    Setup
=====================================================*/
void setup()
{
    Serial.begin(115200);

    initMFRC522();

    Serial.println();
    Serial.println("==============================");
    Serial.println(" MFRC522 Monitoring");
    Serial.println("==============================");
}

/*=====================================================
    Loop
=====================================================*/
void loop()
{
    String uid = getCardUID();

    if (uid == "")
    {
        delay(50);
        return;
    }

    Serial.print("Card UID : ");
    Serial.println(uid);

    if (uid == "79-126-152-189")
    {
        Serial.println("blue card");
    }
    else if (uid == "224-45-214-11")
    {
        Serial.println("white card");
    }
    else
    {
        Serial.println("unknown card");
    }

    Serial.println("----------------------------");

    delay(500);
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
    String uid = "";

    // Kiểm tra có thẻ mới hay không
    if (!mfrc522.PICC_IsNewCardPresent())
        return "";

    // Đọc UID của thẻ
    if (!mfrc522.PICC_ReadCardSerial())
        return "";

    uid = getCardUIDinDEC(
        mfrc522.uid.uidByte,
        mfrc522.uid.size
    );

    // Kết thúc giao tiếp với thẻ
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();

    return uid;
}

/*=====================================================
    Convert UID to HEX
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
    Convert UID to DEC
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