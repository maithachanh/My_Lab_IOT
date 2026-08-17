#include <Adafruit_NeoPixel.h>

/* WS2812B RGB LED Module */

#if defined(ESP8266)
  #define RGB_LED_PIN D6
#elif defined(ESP32)
  #define RGB_LED_PIN 5      // Đổi sang chân bạn đang nối DIN
#endif

#define NUMPIXELS 1

Adafruit_NeoPixel rgbLed(NUMPIXELS, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

//=====================================================
// Setup
//=====================================================
void setup() {
  Serial.begin(115200);
  initRGBLed();
}

//=====================================================
// Loop
//=====================================================
void loop() {

  // Red blinks 10 times
  for (int i = 1; i <= 10; i++) {
    onRGBLed(255, 0, 0);
    delay(500);

    offRGBLed();
    delay(500);
  }

  // Green blinks 7 times
  for (int i = 1; i <= 7; i++) {
    onRGBLed(0, 255, 0);
    delay(500);

    offRGBLed();
    delay(500);
  }

  // Blue blinks 3 times
  for (int i = 1; i <= 3; i++) {
    onRGBLed(0, 0, 255);
    delay(500);

    offRGBLed();
    delay(500);
  }
}

//=====================================================
// Initialize RGB LED
//=====================================================
void initRGBLed() {
  rgbLed.begin();
  rgbLed.clear();
  rgbLed.show();
}

//=====================================================
// Turn on RGB LED
//=====================================================
void onRGBLed(uint8_t red, uint8_t green, uint8_t blue) {
  rgbLed.setPixelColor(0, rgbLed.Color(red, green, blue));
  rgbLed.show();
}

//=====================================================
// Turn off RGB LED
//=====================================================
void offRGBLed() {
  rgbLed.clear();
  rgbLed.show();
}