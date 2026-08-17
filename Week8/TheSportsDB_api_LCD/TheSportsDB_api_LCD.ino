/*
 * =====================================================
 * TheSportsDB API + LCD I2C - ESP32 / ESP8266
 * =====================================================
 *
 * Đấu dây I2C (theo yêu cầu, mặc định cho ESP32):
 *   SCL -> GPIO19  (IO19)
 *   SDA -> GPIO23  (IO23)
 *   GND -> GND
 *   5V  -> 5V (VCC của module LCD I2C)
 *
 * Thư viện cần cài thêm (Sketch > Include Library > Manage Libraries):
 *   - "LiquidCrystal I2C" (tác giả: Frank de Brabander)
 *   - ArduinoJson (bản v7 trở lên, dùng JsonDocument)
 *
 * Cách lấy team_id (theo tài liệu gốc, mục 2.7.3):
 *   Browser: https://www.thesportsdb.com/api/v1/json/123/searchteams.php?t=Manchester%20United
 *   -> lấy "idTeam" trong kết quả trả về, gán vào biến ID_TEAM bên dưới.
 *
 * Ghi chú: LCD 16x2 không hiển thị được dấu tiếng Việt (không dấu),
 * nên toàn bộ chuỗi hiển thị lên LCD được viết không dấu.
 * =====================================================
 */

/* Wifi */
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif

/* TheSportsDB API */
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

/* LCD I2C */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*=====================================================
    WiFi Configuration
=====================================================*/
#define AP_SSID      "HSU_Students"
#define AP_PASSWORD  "tontrongsukhacbiet"

/*=====================================================
    TheSportsDB Configuration
=====================================================*/
// from https://www.thesportsdb.com/api/v1/json/123/searchteams.php?t=XXX
String ID_TEAM = "133612";

/*=====================================================
    I2C / LCD Configuration
=====================================================*/
#if defined(ESP32)
  #define I2C_SDA_PIN   23   // theo yêu cầu: SDA -> IO23
  #define I2C_SCL_PIN   19   // theo yêu cầu: SCL -> IO19
#elif defined(ESP8266)
  // ESP8266 không có chân GPIO19/23, dùng chân I2C mặc định D2/D1.
  // Nếu dùng ESP8266, hãy đấu lại dây theo 2 chân này.
  #define I2C_SDA_PIN   4    // D2
  #define I2C_SCL_PIN   5    // D1
#endif

#define LCD_COLS      16
#define LCD_ROWS      2

LiquidCrystal_I2C* lcd = nullptr;
uint8_t lcdAddress = 0x27;    // sẽ được dò lại tự động trong setup()

/*=====================================================
    Thời gian (non-blocking, không dùng delay() dài)
=====================================================*/
const unsigned long MATCH_INTERVAL   = 10000;   // 10 giây: gọi API 1 lần (giống code gốc)
const unsigned long DISPLAY_INTERVAL = 3000;    // 3 giây: đổi trang màn hình

unsigned long lastMatchFetch    = 0;
unsigned long lastDisplaySwitch = 0;
uint8_t       displayPage       = 0;
const uint8_t TOTAL_PAGES       = 7;

/*=====================================================
    Dữ liệu trận đấu (lưu toàn cục để LCD dùng lại)
=====================================================*/
bool   lastMatchAvailable = false;
String lastRound = "", lastDate = "", lastHome = "", lastAway = "";
String lastScoreHome = "", lastScoreAway = "";

bool   nextMatchAvailable = false;
String nextRound = "", nextDate = "", nextHome = "", nextAway = "";

/*=====================================================
    Function Prototype
=====================================================*/
void initWifi();
void getLastMatch();
void getNextMatch();
void initLCD();
uint8_t scanI2CAddress();
void lcdMessage(const String &line1, const String &line2 = "");
void updateDisplay();

/*=====================================================
    Setup - chạy 1 lần
=====================================================*/
void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);   // SDA, SCL
  initLCD();

  lcdMessage("Dang ket noi", "WiFi...");
  initWifi();

  lcdMessage("WiFi da ket noi", WiFi.localIP().toString());
  delay(1500);
}

/*=====================================================
    Loop - chạy lặp lại
=====================================================*/
void loop() {
  unsigned long now = millis();

  if (WiFi.status() != WL_CONNECTED) {
    lcdMessage("Mat ket noi WiFi", "Dang ket noi lai");
    initWifi();
  }

  // Gọi API mỗi MATCH_INTERVAL (và ngay lần đầu tiên) - giống delay(10000) ở code gốc
  if (lastMatchFetch == 0 || now - lastMatchFetch >= MATCH_INTERVAL) {
    lastMatchFetch = now;

    getLastMatch();
    getNextMatch();

    // Có dữ liệu mới -> quay lại trang đầu và hiển thị ngay
    displayPage = 0;
    lastDisplaySwitch = now;
    updateDisplay();
  }

  // Đổi trang hiển thị trên LCD mỗi DISPLAY_INTERVAL
  if (now - lastDisplaySwitch >= DISPLAY_INTERVAL) {
    lastDisplaySwitch = now;
    displayPage = (displayPage + 1) % TOTAL_PAGES;
    updateDisplay();
  }
}

/*=====================================================
    Khởi tạo LCD I2C (tự dò địa chỉ I2C)
=====================================================*/
void initLCD() {
  lcdAddress = scanI2CAddress();

  lcd = new LiquidCrystal_I2C(lcdAddress, LCD_COLS, LCD_ROWS);
  lcd->init();
  lcd->backlight();
  lcd->clear();
}

/*=====================================================
    Dò địa chỉ I2C của LCD
    (địa chỉ thường gặp: 0x27 hoặc 0x3F tùy loại module)
=====================================================*/
uint8_t scanI2CAddress() {
  Serial.println("Dang do dia chi I2C...");

  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Tim thay thiet bi I2C tai dia chi: 0x");
      Serial.println(addr, HEX);
      return addr;
    }
  }

  Serial.println("Khong tim thay thiet bi I2C nao! Dung dia chi mac dinh 0x27.");
  return 0x27;
}

/*=====================================================
    Hiển thị 1-2 dòng lên LCD (tự cắt bớt nếu quá dài)
=====================================================*/
void lcdMessage(const String &line1, const String &line2) {
  if (!lcd) return;

  lcd->clear();

  lcd->setCursor(0, 0);
  lcd->print(line1.substring(0, LCD_COLS));

  if (line2.length() > 0) {
    lcd->setCursor(0, 1);
    lcd->print(line2.substring(0, LCD_COLS));
  }
}

/*=====================================================
    Cập nhật LCD theo trang hiện tại (xoay vòng thông tin)
    Trang 0-2 : Trận vừa đấu xong (getLastMatch)
    Trang 3-5 : Trận sắp diễn ra (getNextMatch)
    Trang 6   : Địa chỉ IP (tiện debug)
=====================================================*/
void updateDisplay() {
  switch (displayPage) {
    case 0:
      if (lastMatchAvailable)
        lcdMessage("Tran vua qua:", "Vong " + lastRound);
      else
        lcdMessage("Tran vua qua:", "Dang cho...");
      break;

    case 1:
      if (lastMatchAvailable)
        lcdMessage("Nha: " + lastHome, "Khach: " + lastAway);
      else
        lcdMessage("Dang cho du lieu");
      break;

    case 2:
      if (lastMatchAvailable)
        lcdMessage("Ty so: " + lastScoreHome + "-" + lastScoreAway, lastDate);
      else
        lcdMessage("Dang cho du lieu");
      break;

    case 3:
      if (nextMatchAvailable)
        lcdMessage("Tran sap toi:", "Vong " + nextRound);
      else
        lcdMessage("Tran sap toi:", "Dang cho...");
      break;

    case 4:
      if (nextMatchAvailable)
        lcdMessage("Nha: " + nextHome, "Khach: " + nextAway);
      else
        lcdMessage("Dang cho du lieu");
      break;

    case 5:
      if (nextMatchAvailable)
        lcdMessage("Ngay thi dau:", nextDate);
      else
        lcdMessage("Dang cho du lieu");
      break;

    case 6:
      lcdMessage("Dia chi IP:", WiFi.localIP().toString());
      break;
  }
}

/*=====================================================
    Connect WiFi
=====================================================*/
void initWifi() {
  WiFi.mode(WIFI_STA);

#if defined(ESP32)
  WiFi.setSleep(false);      // Giữ WiFi ổn định trên ESP32
#endif

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(AP_SSID);

  WiFi.begin(AP_SSID, AP_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("=================================");
  Serial.println("WiFi Connected");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
  Serial.println("=================================");
}

/*=====================================================
    TheSportsDB - Trận đấu gần nhất (eventslast.php)
=====================================================*/
void getLastMatch() {
  String url = "https://www.thesportsdb.com/api/v1/json/123/eventslast.php?id=" + ID_TEAM;

  HTTPClient https;
  WiFiClientSecure client;
  client.setInsecure();
  https.begin(client, url);

  int httpCode = https.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = https.getString();
    Serial.println(payload);  // for DEBUG

    JsonDocument data;
    DeserializationError error = deserializeJson(data, payload);

    if (!error) {
      JsonArray events = data["results"];

      if (events.size() > 0) {
        lastRound     = events[0]["intRound"].as<String>();
        lastDate      = events[0]["dateEvent"].as<String>();
        lastHome      = events[0]["strHomeTeam"].as<String>();
        lastAway      = events[0]["strAwayTeam"].as<String>();
        lastScoreHome = events[0]["intHomeScore"].as<String>();
        lastScoreAway = events[0]["intAwayScore"].as<String>();

        lastMatchAvailable = true;

        String result = "(round " + lastRound + ", " + lastDate + "): " +
                         lastHome + " " + lastScoreHome + " - " +
                         lastScoreAway + " " + lastAway;
        Serial.println("Last match " + result);
      }
    } else {
      Serial.println("JSON error: " + String(error.c_str()));
    }
  } else {
    Serial.println("HTTP error: " + String(httpCode));
    lcdMessage("Loi API", "Last: " + String(httpCode));
  }

  https.end();
}

/*=====================================================
    TheSportsDB - Trận đấu sắp tới (eventsnext.php)
=====================================================*/
void getNextMatch() {
  String url = "https://www.thesportsdb.com/api/v1/json/123/eventsnext.php?id=" + ID_TEAM;

  HTTPClient https;
  WiFiClientSecure client;
  client.setInsecure();
  https.begin(client, url);

  int httpCode = https.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = https.getString();
    Serial.println(payload);  // for DEBUG

    JsonDocument data;
    DeserializationError error = deserializeJson(data, payload);

    if (!error) {
      JsonArray events = data["events"];

      if (events.size() > 0) {
        nextRound = events[0]["intRound"].as<String>();
        nextDate  = events[0]["dateEvent"].as<String>();
        nextHome  = events[0]["strHomeTeam"].as<String>();
        nextAway  = events[0]["strAwayTeam"].as<String>();

        nextMatchAvailable = true;

        String result = "(round " + nextRound + ", " + nextDate + "): " +
                         nextHome + " - " + nextAway;
        Serial.println("Next match " + result);
      }
    } else {
      Serial.println("JSON error: " + String(error.c_str()));
    }
  } else {
    Serial.println("HTTP error: " + String(httpCode));
    lcdMessage("Loi API", "Next: " + String(httpCode));
  }

  https.end();
}
