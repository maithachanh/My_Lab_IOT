/* Light sensor */
#if defined(ESP8266)
  #define DO_PIN D5
#elif defined(ESP32)
  #define DO_PIN 13
#endif

/* Wifi */
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif
#define AP_SSID "HSU_Students"
#define AP_PASSWORD "tontrongsukhacbiet"

/* Epoch time */
#include <time.h>
#define NTP_SERVER "sg.pool.ntp.org"

/* Blynk cloud */
#define BLYNK_TEMPLATE_ID   "TMPL66RWIdLQD"
#define BLYNK_TEMPLATE_NAME "DemoLightSensor"
#define BLYNK_AUTH_TOKEN    "vlBsnuWMeKKF98BTXCYsIJ089-2WkScy"

#if defined(ESP8266)
#include <BlynkSimpleEsp8266.h>
#elif defined(ESP32)
#include <BlynkSimpleEsp32.h>
#endif

// setup to run only once
void setup() {
    Serial.begin(115200);

    initSensor();
    initWifi();
    initEpochTime();

    initBlynk();
}

// loop to run repeatedly
void loop() {
    Blynk.run();    // keep the connection to the server

    bool light = readDO();

    if (light) {
        // Serial.println("Light: +");
        setLightToBlynk(light);
    }
    else {
        // Serial.println("Light: -");
        setLightToBlynk(light);
    }

    delay(1000);
}

/* Light sensor */
void initSensor() {
  pinMode(DO_PIN, INPUT);
}

bool readDO() {
  int dValue = digitalRead(DO_PIN);   // 0: có ánh sáng, 1: tối (không có ánh sáng)

  if (dValue)
    return false; // Trả về false nếu trời tối
  else
    return true;  // Trả về true nếu có ánh sáng
}

/* Wifi */
void initWifi() {
  WiFi.setAutoReconnect(true); 
  WiFi.persistent(true);
  WiFi.begin(AP_SSID, AP_PASSWORD);
  Serial.print("\nConnecting to "); Serial.print(AP_SSID);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  }
  Serial.print("\nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
  Serial.print("MAC address: "); Serial.println(WiFi.macAddress());
}

/* Epoch time */
void initEpochTime() {
  configTime(0, 0, NTP_SERVER); // GMT time offset, daylight saving time, NTP server
  Serial.print("Connecting to "); Serial.print(NTP_SERVER);
  while (getEpochTime() == 0) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nEpoch time: "); Serial.println(getEpochTime());
}

long getEpochTime() {
  time_t now = time(nullptr);
  if (now < 8 * 3600 * 2) { // Nếu thời gian nhỏ hơn một mốc hợp lý (chưa đồng bộ xong NTP)
    return 0;
  }
  return now;
}

/* Blynk cloud */
void initBlynk() {
    Blynk.config(BLYNK_AUTH_TOKEN);

    if (Blynk.connect())
        Serial.println("Blynk connected");
}

void setLightToBlynk(int light) {
    long now = getEpochTime();

    Serial.println(String(light) + " " + String(now));   // for DEBUG

    if (Blynk.connected()) {
        Blynk.virtualWrite(V0, light);
        Blynk.virtualWrite(V1, now);
    }
}