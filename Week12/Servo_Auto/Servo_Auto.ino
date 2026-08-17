/* Servo */
#if defined(ESP8266)
    #include <Servo.h> // ref: https://docs.arduino.cc/learn/electronics/servo-motors
    #define SERVO_PIN D8
#elif defined(ESP32)
    #include <ESP32Servo.h> // ref: https://madhephaestus.github.io/ESP32Servo/annotated.html
    #define SERVO_PIN 16
#endif

Servo servo;

// setup to run only once
void setup() {
    initServo();
}

// loop to run repeatedly
void loop() {
    upServo();
    downServo();
}

/* Servo */
void initServo() {
    servo.attach(SERVO_PIN);
    servo.write(0);
}

void upServo() {
    for (int pos = 0; pos <= 180; pos += 1) { // from 0-180 degrees in steps of 1 degree
        servo.write(pos);
        delay(20);
    }
}

void downServo() {
    for (int pos = 180; pos >= 0; pos -= 1) { // from 180-0 degrees in steps of 1 degree
        servo.write(pos);
        delay(20);
    }
}