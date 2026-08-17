/* ==========================================================
   Active Buzzer
   ========================================================== */

#if defined(ESP8266)
    #define BUZZER_PIN D7
#elif defined(ESP32)
    #define BUZZER_PIN 14
#endif

//==========================================================
// Setup - Chạy một lần
//==========================================================
void setup() {
    initBuzzer();
}

//==========================================================
// Loop - Chạy lặp liên tục
//==========================================================
void loop() {
    sosBuzzer();
    delay(500);
}

//==========================================================
// Khởi tạo Buzzer
//==========================================================
void initBuzzer() {
    pinMode(BUZZER_PIN, OUTPUT);
    offBuzzer();
}

//==========================================================
// Phát tín hiệu SOS (... --- ...)
//==========================================================
void sosBuzzer() {

    // S : 3 tiếng ngắn
    for (int i = 1; i <= 3; i++) {
        digitalWrite(BUZZER_PIN, LOW);
        delay(50);      // Bật

        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);     // Tắt
    }

    delay(100);

    // O : 3 tiếng dài
    for (int i = 1; i <= 3; i++) {
        digitalWrite(BUZZER_PIN, LOW);
        delay(200);     // Bật

        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);     // Tắt
    }

    delay(100);

    // S : 3 tiếng ngắn
    for (int i = 1; i <= 3; i++) {
        digitalWrite(BUZZER_PIN, LOW);
        delay(50);      // Bật

        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);     // Tắt
    }
}

//==========================================================
// Tắt Buzzer
//==========================================================
void offBuzzer() {
    digitalWrite(BUZZER_PIN, HIGH);
}