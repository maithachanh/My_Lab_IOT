/* Light sensor */

#if defined(ESP8266)
  #define DO_PIN D5
#elif defined(ESP32)
  #define DO_PIN 13
#endif

// Setup to run only once
void setup() {
  Serial.begin(115200);
  initSensor();
}

// Loop to run repeatedly
void loop() {
  bool light = readDO();

  if (light) {
    Serial.println("Light: +");
  } else {
    Serial.println("Light: -");
  }

  delay(1000);
}

/* Light sensor */

void initSensor() {
  pinMode(DO_PIN, INPUT);
}

bool readDO() {
  int dValue = digitalRead(DO_PIN);   // 0: light, 1: no-light

  if (dValue)
    return false;
  else
    return true;
}