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

/* Google script */
#if defined(ESP8266)
  #include <ESP8266HTTPClient.h>
#elif defined(ESP32)
  #include <HTTPClient.h>
#endif

#include <WiFiClientSecure.h>

String GSCRIPT_ID = "AKfycbzcBBAQm_7H9ogrZn8SdklqSAyQCC2oodGfM_huki_qs0nPJK71_K1FoO-3syNVok0x"; // Thay thế bằng ID Google Script thực tế của bạn

// Khai báo nguyên mẫu các hàm để tránh lỗi biên dịch (Prototypes)
void initSensor();
bool readDO();
void initWifi();
void initEpochTime();
long getEpochTime();
void setLightToGscript(int light);

// Setup to run only once
void setup() {
  Serial.begin(115200);
  delay(1000); // Chờ Serial khởi động ổn định
  
  initSensor();
  initWifi();
  initEpochTime();
}

// Loop to run repeatedly
void loop() {
  // Đọc trạng thái ánh sáng từ cảm biến (true: sáng, false: tối)
  bool light = readDO();

  // Gửi trạng thái ánh sáng lên Google Script (true truyền vào hàm sẽ tự động ép kiểu thành 1, false thành 0)
  setLightToGscript(light);

  delay(1000); // Chờ 1 giây trước khi lặp lại
}

/* --- Định nghĩa các hàm chức năng --- */

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

/* Google script */
void setLightToGscript(int light) {
  // Kiểm tra kết nối Wifi trước khi thực hiện request để tránh crash mạch
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected! Cannot send data.");
    return;
  }
long now = getEpochTime();
  Serial.println("Light status: " + String(light) + " | Epoch time: " + String(now));   // for DEBUG

  // Khởi tạo URL gửi dữ liệu
  String url = "https://script.google.com/macros/s/" + GSCRIPT_ID + "/exec?value=" + String(light) + "&updated=" + String(now);

  HTTPClient https;
  WiFiClientSecure client;
  client.setInsecure(); // Bỏ qua xác thực chứng chỉ SSL (bắt buộc đối với Google Script trên ESP)

  // Bắt đầu kết nối HTTPS
  if (https.begin(client, url)) {
    // Google Script thường trả về mã chuyển hướng (302 Redirect). 
    // HTTPClient của ESP có thể tự động bắt chuyển hướng hoặc trả về 302, việc gửi GET vẫn thành công.
    int statusCode = https.GET();
    Serial.println("HTTP Status Code: " + String(statusCode));   // for DEBUG
    
    // Nếu muốn xem phản hồi từ Google Script (VD: "Success" hoặc "OK")
    String payload = https.getString();
    Serial.println("Response: " + payload);
    
    https.end();
  } else {
    Serial.println("Unable to connect to Google Script server");
  }
}