/* Relay */

#if defined(ESP8266)
    #define RELAY_PIN D7
#elif defined(ESP32)
    #define RELAY_PIN 13
#endif

// Setup to run only once
void setup()
{
    initRelay();
}

// Loop to run repeatedly
void loop()
{
    onRelay();
    delay(3000);

    offRelay();
    delay(3000);
}

/* Relay */

void initRelay()
{
    pinMode(RELAY_PIN, OUTPUT);
}

void onRelay()
{
    digitalWrite(RELAY_PIN, HIGH);
}

void offRelay()
{
    digitalWrite(RELAY_PIN, LOW);
}