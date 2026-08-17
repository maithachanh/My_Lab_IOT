/* Keypad */
#include <Keypad.h> // ref: https://github.com/Chris--A/Keypad

#define ROWS 4   // four rows
#define COLS 4   // four columns

#if defined(ESP8266)

byte rowPins[ROWS] = {D1, D2, D5, D6}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {D7, D3, D4, D0}; // connect to the column pinouts of the keypad

#elif defined(ESP32)

// byte rowPins[ROWS] = {25, 17, 16, 27};
byte rowPins[ROWS] = {27, 16, 17, 25};  // connect to the row pinouts of the keypad
//byte colPins[COLS] = {18, 19, 23, 5};   
byte colPins[COLS] = {5, 23, 19, 18};  // connect to the column pinouts of the keypad

#endif

char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},  // {'S4', 'S3', 'S2', 'S1'}
    {'4', '5', '6', 'B'},  // {'S8', 'S7', 'S6', 'S5'}
    {'7', '8', '9', 'C'},  // {'S12', 'S11', 'S10', 'S9'}
    {'*', '0', '#', 'D'}   // {'S16', 'S15', 'S14', 'S13'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// setup to run only once
void setup() {
    Serial.begin(115200);
}

// loop to run repeatedly
void loop() {
    char key = keypad.getKey();

    if (key) {
        Serial.println(key);
    }

    delay(100);
}