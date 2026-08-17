#include <DHT.h>

/* DHT11 */
#define DHTPIN 27       // Chân S của DHT11 nối với IO12
#define DHTTYPE DHT11   // Loại cảm biến là DHT11
DHT dht(DHTPIN, DHTTYPE);

/* Relay */
#define RELAY_PIN 23    // Chân S của Relay nối với IO13

// setup to run only once
void setup() {
  Serial.begin(115200);
  dht.begin();          // Khởi tạo cảm biến DHT11
  initRelay();
}

// loop to run repeatedly
int temp = 0, humi = 0;
void loop() {
  readDHT11(temp, humi);
  Serial.println("Temperature: " + String(temp)); // for DEBUG
  Serial.println("Humidity: " + String(humi));    // for DEBUG
  
  if (temp > 25) {
    onRelay();
  } else {
    offRelay();
  }
  
  delay(100);
}

/* Các hàm xử lý chi tiết */

void readDHT11(int& temp, int& humi) {
  // Đọc nhiệt độ và độ ẩm, ép kiểu về số nguyên (int)
  temp = (int)dht.readTemperature();
  humi = (int)dht.readHumidity();
}

/* Relay */
void initRelay() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Đảm bảo Relay tắt lúc khởi động
}

void onRelay() {
  // Bật relay (Tùy loại module relay là mức cao hay thấp, ở đây giả sử là mức CAO/HIGH)
  digitalWrite(RELAY_PIN, HIGH); 
}

void offRelay() {
  // Tắt relay
  digitalWrite(RELAY_PIN, LOW);  
}