/*==========================================================
  Wemos ESP32 + MQ2 + Active Buzzer
==========================================================*/

#define MQ2_PIN      13
#define BUZZER_PIN   14

/*----------------------------------------------------------
  Chỉnh theo module của bạn
----------------------------------------------------------*/

// Nếu Serial luôn hiện "Smoke +" thì đổi false thành true
bool MQ2_ACTIVE_HIGH = true;

// Nếu buzzer không kêu thì đổi false thành true
bool BUZZER_ACTIVE_HIGH = true;


/*==========================================================*/

void setup()
{
    Serial.begin(115200);

    pinMode(MQ2_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    offBuzzer();

    Serial.println();
    Serial.println("==================================");
    Serial.println(" MQ2 GAS DETECTOR ");
    Serial.println("==================================");

    Serial.println("Waiting MQ2 Warm-up...");

    delay(20000);          // MQ2 cần khoảng 20 giây để ổn định

    Serial.println("System Ready.");
}

void loop()
{
    bool smoke = readMQ2();

    Serial.print("MQ2 D0 = ");
    Serial.print(digitalRead(MQ2_PIN));

    if(smoke)
    {
        Serial.println("    Smoke +");

        sosBuzzer();
    }
    else
    {
        Serial.println("    Smoke -");

        offBuzzer();
    }

    delay(500);
}

/*==========================================================*/

bool readMQ2()
{
    int count = 0;

    for(int i=0;i<10;i++)
    {
        if(digitalRead(MQ2_PIN))
            count++;

        delay(5);
    }

    bool value = (count >= 7);

    if(MQ2_ACTIVE_HIGH)
        return value;
    else
        return !value;
}

/*==========================================================*/

void buzzerOn()
{
    digitalWrite(BUZZER_PIN,
                 BUZZER_ACTIVE_HIGH ? HIGH : LOW);
}

void buzzerOff()
{
    digitalWrite(BUZZER_PIN,
                 BUZZER_ACTIVE_HIGH ? LOW : HIGH);
}

void offBuzzer()
{
    buzzerOff();
}

/*==========================================================*/

void beep(int t)
{
    buzzerOn();
    delay(t);

    buzzerOff();
    delay(100);
}

void sosBuzzer()
{
    for(int i=0;i<3;i++)
        beep(100);

    delay(150);

    for(int i=0;i<3;i++)
        beep(300);

    delay(150);

    for(int i=0;i<3;i++)
        beep(100);
}