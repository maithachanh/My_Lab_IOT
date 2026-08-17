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

/* MQTT broker */
#include <PubSubClient.h>   // ref: http://pubsubclient.knolleary.net

#define MQTT_SERVER "mqtt-dashboard.com"   // HiveMQ broker
#define MQTT_PORT 1883
#define MQTT_NAME "ThachAnh_QuocDuy"

WiFiClient client;
PubSubClient mqttClient(client);

// setup to run only once
void setup() {
    Serial.begin(115200);

    initSensor();
    initWifi();
    initEpochTime();

    initMQTT();
}

// loop to run repeatedly
void loop() {
    mqttClient.loop();    // keep the connection to the server

    bool light = readDO();

    if (light) {
        // Serial.println("Light: +");
        publishLightToMQTT(light);
    }
    else {
        // Serial.println("Light: -");
        publishLightToMQTT(light);
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

/* MQTT broker */
void initMQTT() {
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

    Serial.print("Connecting to ");
    Serial.print(MQTT_SERVER);

    String clientId = WiFi.macAddress();

    while (!mqttClient.connected()) {
        Serial.print(".");

        if (mqttClient.connect(clientId.c_str())) {
            Serial.println("\nMQTT connected");
        }
        else {
            Serial.println("\nMQTT error, state: " + String(mqttClient.state()));
        }
    }
}

void publishLightToMQTT(int light) {
    long now = getEpochTime();

    Serial.println(String(light) + " " + String(now));   // for DEBUG

    if (mqttClient.connected()) {
        String topicValue = String(MQTT_NAME) + "/LIGHT/value";
        String topicUpdated = String(MQTT_NAME) + "/LIGHT/updated";

        mqttClient.publish(topicValue.c_str(), String(light).c_str());
        mqttClient.publish(topicUpdated.c_str(), String(now).c_str());
    }
}