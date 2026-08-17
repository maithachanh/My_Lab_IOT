/* Light sensor */
#if defined(ESP8266)
  #define DO_PIN D5
#elif defined(ESP32)
  #define DO_PIN 13
#endif

/* OLED */
#if defined(ESP8266)
    #define SDA_PIN D2
    #define SCL_PIN D1
#elif defined(ESP32)
    #define SDA_PIN 23
    #define SCL_PIN 19
#endif

// Chọn loại màn hình OLED bằng cách bỏ dấu comment (//)
#define ssd1306
// #define sh1106

#if defined(ssd1306)
    #include <SSD1306Wire.h>     // Thư viện mới của ThingPulse
    SSD1306Wire oled(0x3C, SDA_PIN, SCL_PIN);
#elif defined(sh1106)
    #include <SH1106Wire.h>
    SH1106Wire oled(0x3C, SDA_PIN, SCL_PIN);
#endif

// Khởi tạo chạy 1 lần khi cấp nguồn
void setup() {
  Serial.begin(115200);
  
  initSensor();
  initOled();
  
  printOled("System", "Starting...");
  delay(1000);
}

// Vòng lặp chính chạy liên tục
void loop() {
  // Đọc trạng thái từ cảm biến ánh sáng
  bool light = readDO();

  if (light) {
    Serial.println("Light: +");
    printOled("Light sensor", "Light: +");
  } else {
    Serial.println("Light: -");
    printOled("Light sensor", "Light: -");
  }

  delay(1000);
}

/* --- Các hàm bổ trợ viết ở cuối --- */

/* Khởi tạo cảm biến */
void initSensor() {
  pinMode(DO_PIN, INPUT); // Thiết lập chân đọc tín hiệu đầu vào
}

/* Đọc dữ liệu cảm biến */
bool readDO() {
  int dValue = digitalRead(DO_PIN);   // 0: có ánh sáng, 1: tối (không có ánh sáng)

  if (dValue)
    return false; // Trả về false nếu trời tối
  else
    return true;  // Trả về true nếu có ánh sáng
}

/* Khởi tạo màn hình OLED */
void initOled() {
  oled.init();
  oled.flipScreenVertically(); // Xoay màn hình nếu cần, có thể tắt đi bằng cách thêm // ở đầu dòng
}

/* In dữ liệu lên màn hình OLED */
void printOled(String line1, String line2) {
  oled.clear();

  oled.setTextAlignment(TEXT_ALIGN_LEFT);
  oled.setFont(ArialMT_Plain_16); // Đã hạ size từ 24 xuống 16 để tránh bị tràn viền màn hình

  oled.drawString(0, 0, line1);
  oled.drawString(0, 30, line2);

  oled.display();
}