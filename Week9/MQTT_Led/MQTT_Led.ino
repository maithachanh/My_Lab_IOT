#include <WiFi.h>
#include <PubSubClient.h>

//====================================================
// WIFI & MQTT
//====================================================
const char* ssid = "HSU_Students";
const char* password = "tontrongsukhacbiet";

const char* mqtt_server = "broker.hivemq.com";
const char* MQTT_NAME = "22206826";

//====================================================
// RGB LED
//====================================================
#define PIN_RED     5
#define PIN_GREEN   12
#define PIN_BLUE    13

#define PWM_FREQ    5000
#define PWM_RES     8

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// MQTT Topics
String topicR = String(MQTT_NAME) + "/LEDs/R";
String topicG = String(MQTT_NAME) + "/LEDs/G";
String topicB = String(MQTT_NAME) + "/LEDs/B";

// RGB values
uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;

//====================================================
// RGB INIT
//====================================================
void initRGBLed()
{
    ledcAttach(PIN_RED, PWM_FREQ, PWM_RES);
    ledcAttach(PIN_GREEN, PWM_FREQ, PWM_RES);
    ledcAttach(PIN_BLUE, PWM_FREQ, PWM_RES);

    ledcWrite(PIN_RED, 0);
    ledcWrite(PIN_GREEN, 0);
    ledcWrite(PIN_BLUE, 0);
}

//====================================================
// UPDATE RGB
//====================================================
void updateRGB()
{
    // Nếu LED của bạn là Common Cathode
    ledcWrite(PIN_RED, red);
    ledcWrite(PIN_GREEN, green);
    ledcWrite(PIN_BLUE, blue);

    /*
    Nếu LED là Common Anode thì dùng đoạn này:

    ledcWrite(PIN_RED, 255 - red);
    ledcWrite(PIN_GREEN, 255 - green);
    ledcWrite(PIN_BLUE, 255 - blue);
    */
}

//====================================================
// WIFI CONNECT
//====================================================
void connectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
        return;

    Serial.println();
    Serial.println("==============================");
    Serial.println("Connecting WiFi...");
    Serial.println("==============================");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi Connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

//====================================================
// MQTT CALLBACK
//====================================================
void callbackMQTT(char* topic, byte* payload, unsigned int length)
{
    String message = "";

    for (unsigned int i = 0; i < length; i++)
    {
        message += (char)payload[i];
    }

    message.trim();

    if (String(topic) == topicR)
    {
        red = constrain(message.toInt(), 0, 255);
    }
    else if (String(topic) == topicG)
    {
        green = constrain(message.toInt(), 0, 255);
    }
    else if (String(topic) == topicB)
    {
        blue = constrain(message.toInt(), 0, 255);
    }

    updateRGB();

    Serial.println("--------------------------------");
    Serial.println("MQTT Message Received");
    Serial.print("Topic   : ");
    Serial.println(topic);
    Serial.print("Payload : ");
    Serial.println(message);

    Serial.print("R = ");
    Serial.println(red);

    Serial.print("G = ");
    Serial.println(green);

    Serial.print("B = ");
    Serial.println(blue);
    Serial.println("--------------------------------");
}

//====================================================
// MQTT INIT
//====================================================
void initMQTT()
{
    mqttClient.setServer(mqtt_server, 1883);
    mqttClient.setCallback(callbackMQTT);

    mqttClient.setKeepAlive(60);
    mqttClient.setSocketTimeout(20);
}

//====================================================
// MQTT CONNECT
//====================================================
void reconnectMQTT()
{
    while (!mqttClient.connected())
    {
        Serial.print("Connecting MQTT... ");

        String clientID = "ESP32-";
        clientID += String((uint32_t)ESP.getEfuseMac(), HEX);

        if (mqttClient.connect(clientID.c_str()))
        {
            Serial.println("SUCCESS");

            mqttClient.subscribe(topicR.c_str());
            mqttClient.subscribe(topicG.c_str());
            mqttClient.subscribe(topicB.c_str());

            Serial.println("Subscribed Topics:");
            Serial.println(topicR);
            Serial.println(topicG);
            Serial.println(topicB);
        }
        else
        {
            Serial.print("FAILED (rc=");
            Serial.print(mqttClient.state());
            Serial.println(")");

            delay(5000);
        }
    }
}

//====================================================
// SETUP
//====================================================
void setup()
{
    Serial.begin(115200);

    initRGBLed();

    connectWiFi();

    initMQTT();

    reconnectMQTT();
}

//====================================================
// LOOP
//====================================================
void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        connectWiFi();
    }

    if (!mqttClient.connected())
    {
        reconnectMQTT();
    }

    mqttClient.loop();
}