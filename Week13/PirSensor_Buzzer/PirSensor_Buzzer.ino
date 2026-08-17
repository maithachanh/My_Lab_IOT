/* ==========================================================
   Wemos ESP32 + PIR SR501 + Active Buzzer
   ========================================================== */

/* ---------------- PIR Sensor ---------------- */
#define PIR_PIN       13

/* ---------------- Active Buzzer ---------------- */
#define BUZZER_PIN    14

//==========================================================
// Setup
//==========================================================
void setup()
{
    Serial.begin(115200);

    initSensor();
    initBuzzer();

    Serial.println();
    Serial.println("====================================");
    Serial.println(" PIR Motion Detection System");
    Serial.println("====================================");

    Serial.println("Waiting PIR Stabilizing...");
    delay(30000);      // PIR cần khoảng 30 giây để ổn định
    Serial.println("System Ready!");
}

//==========================================================
// Loop
//==========================================================
void loop()
{
    bool motion = readDO();

    if (motion)
    {
        Serial.println("Motion : +");

        sosBuzzer();
    }
    else
    {
        Serial.println("Motion : -");

        offBuzzer();
    }

    delay(100);
}

//==========================================================
// PIR Sensor
//==========================================================

// Khởi tạo cảm biến
void initSensor()
{
    pinMode(PIR_PIN, INPUT);
}

// Đọc trạng thái PIR
bool readDO()
{
    return digitalRead(PIR_PIN) == HIGH;
}

//==========================================================
// Active Buzzer
//==========================================================

// Khởi tạo buzzer
void initBuzzer()
{
    pinMode(BUZZER_PIN, OUTPUT);

    offBuzzer();
}

// Bật buzzer
void onBuzzer()
{
    digitalWrite(BUZZER_PIN, HIGH);
}

// Tắt buzzer
void offBuzzer()
{
    digitalWrite(BUZZER_PIN, LOW);
}

// Phát tín hiệu SOS (... --- ...)
void sosBuzzer()
{
    // S
    for (int i = 0; i < 3; i++)
    {
        onBuzzer();
        delay(100);

        offBuzzer();
        delay(100);
    }

    delay(150);

    // O
    for (int i = 0; i < 3; i++)
    {
        onBuzzer();
        delay(300);

        offBuzzer();
        delay(100);
    }

    delay(150);

    // S
    for (int i = 0; i < 3; i++)
    {
        onBuzzer();
        delay(100);

        offBuzzer();
        delay(100);
    }
}