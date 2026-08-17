#include <WiFi.h>
#include <FirebaseESP32.h>

// Thông tin WiFi và Firebase
#define WIFI_SSID "HSU_Students"
#define WIFI_PASSWORD "tontrongsukhacbiet"
#define FIREBASE_HOST "rgbleddemo-default-rtdb.asia-southeast1.firebasedatabase.app" 
#define FIREBASE_AUTH "nIMhmkgTXnCJk8Gg4pBdUjhEYmaW6CTtIN4nxnZr"

// Định nghĩa chân LED
const int PIN_RED = 23;
const int PIN_GREEN = 19;
const int PIN_BLUE = 18;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

/* Khởi tạo RGB led */
void initRGBLed() {
  // ESP32 Core 3.x.x dùng pinMode bình thường
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  
  // Tắt LED ban đầu
  onRGBLed(0, 0, 0);
}

/* Điều khiển màu LED */
void onRGBLed(int red, int green, int blue) {
  // ESP32 Core 3.x.x hỗ trợ analogWrite rất tốt
  analogWrite(PIN_RED, red);
  analogWrite(PIN_GREEN, green);
  analogWrite(PIN_BLUE, blue);
}

/* Kết nối WiFi */
void initWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected with IP: ");
  Serial.println(WiFi.localIP());
}

/* Khởi tạo Google Firebase */
void initFirebase() {
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

/* Hàm lấy dữ liệu từ Firebase */
void getRGBfromFirebase(int& red, int& green, int& blue) {
  if (Firebase.ready()) {
    // Đọc màu Đỏ
    if (Firebase.getInt(fbdo, "LEDs/R")) red = fbdo.intData();
    delay(20); // QUAN TRỌNG: Nghỉ 20ms để ESP32 giải phóng socket mạng
    
    // Đọc màu Xanh lá
    if (Firebase.getInt(fbdo, "LEDs/G")) green = fbdo.intData();
    delay(20); // QUAN TRỌNG: Chống kẹt request
    
    // Đọc màu Xanh dương
    if (Firebase.getInt(fbdo, "LEDs/B")) blue = fbdo.intData();
    
    Serial.println("RGB: " + String(red) + " " + String(green) + " " + String(blue)); 
  }
}

void setup() {
  Serial.begin(115200);
  initRGBLed();
  initWifi();
  initFirebase();
}

int red = 0, green = 0, blue = 0;

void loop() {
  getRGBfromFirebase(red, green, blue);
  onRGBLed(red, green, blue);
  
  // Tránh spam Firebase liên tục gây block (500ms là chu kỳ an toàn)
  delay(500); 
}