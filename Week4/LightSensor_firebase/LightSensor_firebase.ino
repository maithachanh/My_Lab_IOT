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

/* Google Firebase */
#if defined(ESP8266)
  #include <FirebaseESP8266.h>   // ref: http://github.com/mobizt/Firebase-ESP8266
#elif defined(ESP32)
  #include <FirebaseESP32.h>      // ref: http://github.com/mobizt/Firebase-ESP32
#endif

#define DB_URL    "https://demolightsensor-e88f4-default-rtdb.asia-southeast1.firebasedatabase.app/"      // Realtime Database >> Data >> URL
#define DB_SECRET "RnwfQMZoExnZ5wOWjNmkh7HW2JZDXJAgBYoHuuFM"   // Project settings >> Service accounts >> Database secrets

FirebaseConfig config;
FirebaseAuth auth;
FirebaseData fbdo;   // Firebase data object

void setup() {
    Serial.begin(115200);

    initSensor();
    initWifi();
    initEpochTime();
    initFirebase();
}

void loop() {
    bool light = readDO();

    if (light) {
        // Serial.println("Light: +");
        setLightToFirebase(light);
    }
    else {
        // Serial.println("Light: -");
        setLightToFirebase(light);
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

/* Google Firebase */
void initFirebase() {
    config.database_url = DB_URL;
    config.signer.tokens.legacy_token = DB_SECRET;

    Firebase.begin(&config, &auth);
    Firebase.reconnectNetwork(true);
}

void setLightToFirebase(int light) {
    if (Firebase.ready()) {
        Firebase.setInt(fbdo, "LIGHT/value", light);
        Firebase.setInt(fbdo, "LIGHT/updated", getEpochTime());
    }
}