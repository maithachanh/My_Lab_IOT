/* Flame sensor */

#if defined(ESP8266)
    #define DO_PIN D5
#elif defined(ESP32)
    #define DO_PIN 13
#endif

// setup to run only once
void setup() {
    Serial.begin(115200);
    initSensor();
}

// loop to run repeatedly
void loop() {
    bool flame = readDO();

    if (flame) {
        Serial.println("Flame: +");
    } else {
        Serial.println("Flame: -");
    }

    delay(1000);
}

/* Flame sensor */
void initSensor() {
    pinMode(DO_PIN, INPUT);
}

bool readDO() {
    int dValue = digitalRead(DO_PIN);   // 0: flame; 1: no-flame

    if (dValue)
        return false;
    else
        return true;
}