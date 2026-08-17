/* Light sensor */

#if defined(ESP8266)
  #define AO_PIN A0
#elif defined(ESP32)
  #define AO_PIN 34
#endif

// Setup to run only once
void setup() {
  Serial.begin(115200);
}

// Loop to run repeatedly
void loop() {
  int light = readAO();
  Serial.println("Light: " + String(light) + "%");
  delay(1000);
}

/* Light sensor */

int readAO() {
  int aValue = analogRead(AO_PIN);  // Read analog value

#if defined(ESP8266)
  int light = map(aValue, 0, 1024, 0, 100);   // Convert 0-1024 to 0-100%
#elif defined(ESP32)
  int light = map(aValue, 0, 4095, 0, 100);   // Convert 0-4095 to 0-100%
#endif

  return light;
}