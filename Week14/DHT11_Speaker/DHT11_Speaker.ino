/*
 * DHT11 + WiFi + AudioI2S
 * ESP32 + MAX98357A + Speaker
 */

// ============================================================
// DHT11
// ============================================================

#include <DHT.h>

// DHT11 DATA pin
#define DHT_PIN 13

// DHT sensor type
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);


// ============================================================
// WiFi
// ============================================================

#include <WiFi.h>

// Thay bằng WiFi của bạn
#define WIFI_SSID     "HSU_Students"
#define WIFI_PASSWORD "tontrongsukhacbiet"


// ============================================================
// AudioI2S
// ============================================================

#include <Audio.h>

Audio audio;

// MAX98357A I2S pins
#define LRC_PIN  18
#define BCLK_PIN 19
#define DIN_PIN  23


// ============================================================
// SETUP
// ============================================================

void setup() {

  // Start Serial Monitor
  Serial.begin(115200);

  // Initialize DHT11
  dht.begin();

  // Initialize WiFi
  initWiFi();

  // Initialize Audio
  initAudio();
}


// ============================================================
// LOOP
// ============================================================

long last = 0;

void loop() {

  // Required to keep audio playback running smoothly
  audio.loop();

  // Read sensor every 10 seconds
  if (!audio.isRunning() && millis() - last >= 10000) {

    int temp = 0;
    int humi = 0;

    // Read DHT11
    readDHT11(temp, humi);

    // Print result to Serial Monitor
    Serial.println(
      "Temperature: " +
      String(temp) +
      " - Humidity: " +
      String(humi)
    );

    // Create text for Text-To-Speech
    String text =
      "Nhiệt độ là " +
      String(temp) +
      " độ C và độ ẩm là " +
      String(humi) +
      " phần trăm";

    // Convert text to speech
    text2speech(text);

    // Update timer
    last = millis();
  }
}


// ============================================================
// DHT11
// ============================================================

void readDHT11(int& temp, int& humi) {

  // Read humidity
  float humidity = dht.readHumidity();

  // Read temperature in Celsius
  float temperature = dht.readTemperature();

  // Check if reading failed
  if (isnan(humidity) || isnan(temperature)) {

    Serial.println("Failed to read from DHT11!");

    temp = 0;
    humi = 0;

    return;
  }

  // Convert float to integer
  temp = (int)temperature;
  humi = (int)humidity;
}


// ============================================================
// WiFi
// ============================================================

void initWiFi() {

  Serial.println();
  Serial.println("Connecting to WiFi...");

  // Set ESP32 to Station mode
  WiFi.mode(WIFI_STA);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}


// ============================================================
// AudioI2S
// ============================================================

void initAudio() {

  // Configure I2S pins
  audio.setPinout(
    BCLK_PIN,
    LRC_PIN,
    DIN_PIN
  );

  // Set volume
  // Maximum volume in this library is normally 21
  audio.setVolume(21);

  Serial.println("Audio I2S initialized.");
}


// ============================================================
// TEXT TO SPEECH
// ============================================================

void text2speech(String text) {

  Serial.println("Playing audio...");
  Serial.print("Text: ");
  Serial.println(text);

  // Google Translate Text-To-Speech URL
  String url =
    "https://translate.google.com/translate_tts"
    "?client=tw-ob"
    "&tl=vi"
    "&q=" +
    urlEncode(text);

  Serial.println("TTS URL:");
  Serial.println(url);

  // Connect to Google TTS
  audio.connecttohost(url.c_str());
}


// ============================================================
// URL ENCODE
// ============================================================

String urlEncode(String text) {

  String encoded = "";

  for (char c : text) {

    // Characters that do not need URL encoding
    if (isalnum((unsigned char)c) ||
        c == '-' ||
        c == '_' ||
        c == '.' ||
        c == '~') {

      encoded += c;
    }

    else {

      char hex[4];

      sprintf(
        hex,
        "%%%02X",
        (unsigned char)c
      );

      encoded += hex;
    }
  }

  return encoded;
}