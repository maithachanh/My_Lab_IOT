/* ==========================================================
   Wemos D1 ESP32 + Sound Sensor + RGB LED 4 chân
   Fixed Version
   ========================================================== */

/* ---------------- Sound Sensor ---------------- */
#if defined(ESP8266)
    #define DO_PIN D5
#elif defined(ESP32)
    #define DO_PIN 27
#endif

/* ---------------- RGB LED ---------------- */
#if defined(ESP8266)
    #define RED_PIN     D7
    #define GREEN_PIN   D6
    #define BLUE_PIN    D5
#elif defined(ESP32)
    #define RED_PIN     23
    #define GREEN_PIN   19
    #define BLUE_PIN    18
#endif

/* ========================================================== */

int clap = 0;

unsigned long detection_range_start = 0;
unsigned long detection_range = 0;
unsigned long lastClapTime = 0;

bool lastState = HIGH;

/* ========================================================== */

void setup()
{
    Serial.begin(115200);

    initSensor();
    initRGBLed();

    setColor(0, 0, 0);

    Serial.println("System Starting...");
    delay(2000);              // Đợi cảm biến ổn định
    Serial.println("Ready");
}

/* ========================================================== */

void loop()
{
    readDO();
}

/* ==========================================================
   SOUND SENSOR
   ========================================================== */

void initSensor()
{
    pinMode(DO_PIN, INPUT_PULLUP);
}

void readDO()
{
    bool currentState = digitalRead(DO_PIN);

    // Chỉ phát hiện cạnh HIGH -> LOW
    if (lastState == HIGH && currentState == LOW)
    {
        // Debounce 150 ms
        if (millis() - lastClapTime > 150)
        {
            lastClapTime = millis();

            if (clap == 0)
            {
                detection_range_start = millis();
            }

            detection_range = millis();
            clap++;

            Serial.print("Clap Detected: ");
            Serial.println(clap);
        }
    }

    lastState = currentState;

    // Sau 400ms không có clap mới thì xử lý
    if (clap > 0 && (millis() - detection_range >= 400))
    {
        Serial.println("--------------------");
        Serial.print("Total Clap = ");
        Serial.println(clap);

        switch (clap)
        {
            case 1:
                Serial.println("WHITE");
                setColor(255, 255, 255);
                break;

            case 2:
                Serial.println("OFF");
                setColor(0, 0, 0);
                break;

            default:
                Serial.println("Unknown Command");
                break;
        }

        Serial.println("--------------------");

        clap = 0;
        detection_range = 0;
        detection_range_start = 0;
    }
}

/* ==========================================================
   RGB LED
   ========================================================== */

void initRGBLed()
{
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

#if defined(ESP32)

    ledcAttach(RED_PIN, 5000, 8);
    ledcAttach(GREEN_PIN, 5000, 8);
    ledcAttach(BLUE_PIN, 5000, 8);

#endif

    setColor(0, 0, 0);
}

void setColor(int red, int green, int blue)
{
#if defined(ESP32)

    ledcWrite(RED_PIN, red);
    ledcWrite(GREEN_PIN, green);
    ledcWrite(BLUE_PIN, blue);

#else

    analogWrite(RED_PIN, red);
    analogWrite(GREEN_PIN, green);
    analogWrite(BLUE_PIN, blue);

#endif
}