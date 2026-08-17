/* Servo */
#include <ESP32Servo.h>
Servo servo;
const int servoPin = 16; // Chân tín hiệu nối với dây Vàng (IO16)

/* Wifi */
#include <WiFi.h>
#define WIFI_SSID "HSU_Students"
#define WIFI_PASSWORD "tontrongsukhacbiet"

/* Google Firebase */
#include <FirebaseESP32.h>
#define FIREBASE_HOST "demoservo-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "sd2b7hwYXnbayyMzn4YP8fDAvzSagI5URx5bbvBX" // Sử dụng DB Secret

// Khai báo thêm các đối tượng cấu hình cho thư viện bản mới
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Khai báo biến lưu trạng thái cũ
int oldValue = 0;

// setup to run only once
void setup() {
  Serial.begin(115200);
  initServo();
  initWifi();
  initFirebase();
  setValueToFirebase(0);
}

// loop to run repeatedly
void loop() {
  int newValue = getValueFromFirebase(); // 0:down, 1:up
  
  if (newValue == 1 && oldValue == 0) {
    upServo();
    oldValue = 1;
  } else if (newValue == 0 && oldValue == 1) {
    downServo();
    oldValue = 0;
  }
  
  delay(100);
}

/* Servo */
void initServo() {
  servo.attach(servoPin);
  servo.write(0); // Đưa servo về góc 0 độ khi khởi động
  Serial.println("Servo Initialized at 0 degrees.");
}

void upServo() {
  servo.write(180);
  Serial.println("=> Action: Servo UP (180 degrees)");
}

void downServo() {
  servo.write(0);
  Serial.println("=> Action: Servo DOWN (0 degrees)");
}

/* Wifi */
void initWifi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());
}

/* Google Firebase */
void initFirebase() {
  // Gán thông tin vào đối tượng config (Cách viết của thư viện mới)
  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH; // Cấu hình dùng Database Secret

  // Khởi tạo Firebase với cấu hình mới
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase Connected");
}

void setValueToFirebase(int value) {
  if (Firebase.ready()) {
    Firebase.setInt(fbdo, "SERVO/value", value);
  }
}

int getValueFromFirebase() {
  int value = -1;
  if (Firebase.ready()) {
    if (Firebase.getInt(fbdo, "SERVO/value")) {
      value = fbdo.intData();
    }
  }
  return value;
}