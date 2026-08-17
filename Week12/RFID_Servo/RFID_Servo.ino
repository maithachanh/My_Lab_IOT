/*******************************************************
 * Project : RFID Door Lock (Toggle)
 * Board   : Wemos D1 ESP32
 * RFID    : MFRC522
 * Servo   : SG90
 *******************************************************/

#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

/******************** Pin ********************/
#define SS_PIN      13
#define RST_PIN     18
#define SERVO_PIN   12

/******************** Object ********************/
MFRC522 rfid(SS_PIN, RST_PIN);
Servo servo;

/******************** Authorized UID ********************/
// Thay bằng UID của bạn
String UID_CARD1 = "4F 7E 98 BD";
String UID_CARD2 = "E0 2D D6 0B";

/******************** Servo Angle ********************/
const int CLOSE_ANGLE = 0;
const int OPEN_ANGLE  = 180;

/******************** Variables ********************/
bool doorOpened = false;
unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL = 1000;   // 1 giây

/*******************************************************/

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("===============================");
    Serial.println("   RFID SERVO DOOR LOCK");
    Serial.println("===============================");

    // SPI
    SPI.begin(5, 19, 23, 13);

    // RFID
    rfid.PCD_Init();

    // Servo
    servo.setPeriodHertz(50);
    servo.attach(SERVO_PIN, 500, 2400);

    servo.write(CLOSE_ANGLE);
    delay(500);

    Serial.println("MFRC522 Initialized");
    Serial.println("Servo Initialized");
    Serial.println("Waiting for RFID Card...");
}

/*******************************************************/

void loop()
{
    // Chống đọc liên tục
    if (millis() - lastReadTime < READ_INTERVAL)
        return;

    // Không có thẻ
    if (!rfid.PICC_IsNewCardPresent())
        return;

    // Không đọc được UID
    if (!rfid.PICC_ReadCardSerial())
        return;

    lastReadTime = millis();

    String uid = "";

    for (byte i = 0; i < rfid.uid.size; i++)
    {
        if (rfid.uid.uidByte[i] < 0x10)
            uid += "0";

        uid += String(rfid.uid.uidByte[i], HEX);

        if (i < rfid.uid.size - 1)
            uid += " ";
    }

    uid.toUpperCase();

    Serial.println();
    Serial.print("Card UID : ");
    Serial.println(uid);

    if (uid == UID_CARD1 || uid == UID_CARD2)
    {
        Serial.println("Access Granted");

        if (!doorOpened)
        {
            openDoor();
            doorOpened = true;
        }
        else
        {
            closeDoor();
            doorOpened = false;
        }
    }
    else
    {
        Serial.println("Access Denied");
    }

    // Kết thúc phiên giao tiếp RFID
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    delay(200);
}

/*******************************************************
                    OPEN DOOR
*******************************************************/
void openDoor()
{
    Serial.println("Opening Door...");

    servo.write(OPEN_ANGLE);

    delay(600);

    Serial.println("Door Opened");
}

/*******************************************************
                    CLOSE DOOR
*******************************************************/
void closeDoor()
{
    Serial.println("Closing Door...");

    servo.write(CLOSE_ANGLE);

    delay(600);

    Serial.println("Door Closed");
}