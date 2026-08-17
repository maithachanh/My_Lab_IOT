/* Bluetooth A2DP */

#include <AudioTools.h>          // https://github.com/pschatzmann/arduino-audio-tools
#include <BluetoothA2DPSink.h>   // https://github.com/pschatzmann/ESP32-A2DP

I2SStream i2s;
BluetoothA2DPSink a2dp(i2s);

// =============================
// Khai báo chân I2S
// =============================
#define LRC_PIN  18
#define BCLK_PIN 19
#define DIN_PIN  23

// Tên Bluetooth của ESP32
#define DEVICE_NAME "ESP32@MaiAnh"

// =============================
// Hàm setup
// =============================
void setup() {
  Serial.begin(115200);

  initA2DP();
}

// =============================
// Hàm loop
// =============================
void loop() {
}

// =============================
// Khởi tạo Bluetooth A2DP + I2S
// =============================
void initA2DP() {

  // Lấy cấu hình mặc định của I2S
  I2SConfig cfg = i2s.defaultConfig();

  // Thiết lập các chân I2S
  cfg.pin_bck  = BCLK_PIN;
  cfg.pin_ws   = LRC_PIN;
  cfg.pin_data = DIN_PIN;

  // Khởi động I2S
  i2s.begin(cfg);

  // Khởi động Bluetooth A2DP
  a2dp.start(DEVICE_NAME);

  // Thông báo trên Serial Monitor
  Serial.println("Bluetooth Speaker is ready to pair");
}