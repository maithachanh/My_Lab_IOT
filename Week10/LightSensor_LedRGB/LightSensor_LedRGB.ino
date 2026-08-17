/*=========================================================
  Wemos D1 ESP32 + RGB LED 4 chân + Light Sensor
=========================================================*/

/* Light Sensor */
#define DO_PIN      16

/* RGB LED */
#define RED_PIN     23
#define GREEN_PIN   19
#define BLUE_PIN    18

/*=========================================================*/

void initSensor();
bool readDO();

void initRGBLed();
void setRGB(bool r, bool g, bool b);

/*=========================================================*/

void setup()
{
    Serial.begin(115200);

    initSensor();
    initRGBLed();
}

/*=========================================================*/

void loop()
{
    bool light = readDO();

    if (light)
    {
        Serial.println("Light Detected");

        // LED màu xanh lá
        setRGB(LOW, LOW, LOW);
    }
    else
    {
        Serial.println("Dark");

        // LED màu đỏ
        setRGB(HIGH, HIGH, HIGH);
    }

    delay(200);
}

/*=========================================================
                      LIGHT SENSOR
=========================================================*/

void initSensor()
{
    pinMode(DO_PIN, INPUT);
}

bool readDO()
{
    int value = digitalRead(DO_PIN);

    // LM393:
    // 0 = Có ánh sáng
    // 1 = Tối

    if(value == LOW)
        return true;
    else
        return false;
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