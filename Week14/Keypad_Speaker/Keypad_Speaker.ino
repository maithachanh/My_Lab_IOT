/*
 * Keypad + WiFi + AudioI2S
 * ESP32 + MAX98357A + Speaker
 */

// ============================================================
// KEYPAD
// ============================================================

#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// R1, R2, R3, R4
byte rowPins[ROWS] = {
  14,
  12,
  13,
  5
};

// C1, C2, C3, C4
byte colPins[COLS] = {
  27,
  16,
  17,
  25
};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);


// ============================================================
// WIFI
// ============================================================

#include <WiFi.h>

#define WIFI_SSID     "HSU_Students"
#define WIFI_PASSWORD "tontrongsukhacbiet"


// ============================================================
// AUDIO I2S
// ============================================================

#include <Audio.h>

Audio audio;

// MAX98357A
#define LRC_PIN  18
#define BCLK_PIN 19
#define DIN_PIN  23


// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);

  // Initialize WiFi
  initWiFi();

  // Initialize Audio I2S
  initAudio();

  Serial.println();
  Serial.println("==============================");
  Serial.println(" ESP32 Keypad TTS");
  Serial.println("==============================");
  Serial.println("Enter numbers using keypad.");
  Serial.println("Press * or # to speak.");
}


// ============================================================
// LOOP
// ============================================================

long last = 0;

String text = "";

void loop() {

  // Required to keep audio playback running smoothly
  audio.loop();

  // Read keypad
  char key = keypad.getKey();

  // Check every 100 ms
  if (key != NO_KEY && !audio.isRunning()) {

    // Only process after the required delay
    if (millis() - last >= 100) {

      // ==========================================
      // Number keys
      // ==========================================

      if ('0' <= key && key <= '9') {

        Serial.print(key);

        // Add key to text
        text += key;
      }

      // ==========================================
      // * or #
      // ==========================================

      else if (key == '*' || key == '#') {

        Serial.println();

        Serial.print("Text: ");
        Serial.println(text);

        // Speak the entered number
        if (text.length() > 0) {

          text2speech(text);
        }

        // Clear text
        text = "";
      }

      // Update timer
      last = millis();
    }
  }
}


// ============================================================
// WIFI
// ============================================================

void initWiFi() {

  Serial.println();
  Serial.println("Connecting to WiFi...");

  WiFi.mode(WIFI_STA);

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD
  );

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
// AUDIO I2S
// ============================================================

void initAudio() {

  // Configure MAX98357A I2S pins
  audio.setPinout(
    BCLK_PIN,
    LRC_PIN,
    DIN_PIN
  );

  // Set volume
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

  // Google Translate Text-To-Speech
  String url =
    "https://translate.google.com/translate_tts"
    "?client=tw-ob"
    "&tl=vi"
    "&q=" +
    urlEncode(text);

  Serial.println("TTS URL:");
  Serial.println(url);

  // Connect to Google TTS
  audio.connecttohost(
    url.c_str()
  );
}


// ============================================================
// URL ENCODE
// ============================================================

String urlEncode(String text) {

  String encoded = "";

  for (char c : text) {

    // Characters that can remain unchanged
    if (
      isalnum((unsigned char)c) ||
      c == '-' ||
      c == '_' ||
      c == '.' ||
      c == '~'
    ) {

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