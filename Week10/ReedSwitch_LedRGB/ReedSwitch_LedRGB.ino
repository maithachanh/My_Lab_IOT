/*=========================================================
    Reed Switch + RGB LED
    Board : Wemos D1 ESP32
=========================================================*/

/* Reed Switch */
#define REED_PIN    13

/* RGB LED */
#define RED_PIN     23
#define GREEN_PIN   19
#define BLUE_PIN    18

/*=========================================================*/

void initReed();
bool readReed();

void initRGBLed();
void setRGB(bool r, bool g, bool b);

/*=========================================================*/

void setup()
{
    Serial.begin(115200);

    initReed();
    initRGBLed();
}

/*=========================================================*/

void loop()
{
    bool doorClosed = readReed();

    if (doorClosed)
    {
        Serial.println("Magnet Detected (Door Close)");

        // LED màu xanh lá
        setRGB(HIGH, HIGH, HIGH);
    }
    else
    {
        Serial.println("Magnet Removed (Door Open)");

        // LED màu đỏ
        setRGB(LOW, LOW, LOW);
    }

    delay(200);
}

/*=========================================================
                    REED SWITCH
=========================================================*/

void initReed()
{
    pinMode(REED_PIN, INPUT_PULLUP);
}

bool readReed()
{
    return digitalRead(REED_PIN) == LOW;
}

/*=========================================================
                      RGB LED
=========================================================*/

void initRGBLed()
{
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    setRGB(LOW, LOW, LOW);
}

void setRGB(bool r, bool g, bool b)
{
    digitalWrite(RED_PIN, r);
    digitalWrite(GREEN_PIN, g);
    digitalWrite(BLUE_PIN, b);
}