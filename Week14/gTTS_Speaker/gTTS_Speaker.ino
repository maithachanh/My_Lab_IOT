/*
 * ESP32 WiFi + AudioI2S + Text To Speech
 * MAX98357A I2S Audio Amplifier
 *
 * Library:
 * ESP32-audioI2S
 * https://github.com/schreibfaul1/ESP32-audioI2S
 *
 * Version: 3.2.1
 */

#include <WiFi.h>
#include <Audio.h>

// ============================================================
// WIFI
// ============================================================

#define WIFI_SSID     "HSU_Students"
#define WIFI_PASSWORD "tontrongsukhacbiet"

// ============================================================
// AudioI2S
// ============================================================

Audio audio;

// MAX98357A I2S pins
#define LRC_PIN  18
#define BCLK_PIN 19
#define DIN_PIN  23

// ============================================================
// SETUP
// ============================================================

void setup() {

  // Serial Monitor
  Serial.begin(115200);

  // Connect to WiFi
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

  // Speak every 10 seconds if audio is not currently playing
  if (!audio.isRunning() && millis() - last >= 10000) {

    text2speech("Xin chào tôi là Mai Thạch Anh");

    last = millis();
  }
}

// ============================================================
// WIFI
// ============================================================

void initWiFi() {

  Serial.println();
  Serial.println("Connecting to WiFi...");

  WiFi.mode(WIFI_STA);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

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
  // Range normally 0 - 21
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

  // Connect ESP32 to Google TTS
  audio.connecttohost(url.c_str());
}

// ============================================================
// URL ENCODE
// ============================================================

String urlEncode(String text) {

  String encoded = "";

  for (char c : text) {

    // Keep normal URL characters
    if (isalnum(c) || strchr("-_.~", c)) {

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