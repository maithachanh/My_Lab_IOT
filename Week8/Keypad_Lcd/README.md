# Keypad_Lcd

## Mô tả dự án
Dự án này sử dụng board ESP8266 hoặc ESP32 để đọc tín hiệu từ bàn phím mã hóa 4x4 và hiển thị phím vừa bấm lên màn hình LCD1602 I2C. Ngoài ra, ký tự được gửi ra Serial Monitor để người dùng dễ theo dõi.

## Thành phần phần cứng
- 1 board ESP8266 hoặc ESP32
- 1 module keypad 4x4
- 1 màn hình LCD1602 I2C
- 1 bộ dây nối
- 1 nguồn điện phù hợp

## Cấu trúc file
- Keypad_Lcd.ino: file chương trình chính
- README.md: hướng dẫn sử dụng

## Cách đấu dây

### 1. Kết nối Keypad 4x4

| Chân dây | ESP8266 | ESP32 |
|---|---|---|
| C4 | D0 | GPIO18 |
| C3 | D4 | GPIO19 |
| C2 | D3 | GPIO23 |
| C1 | D7 | GPIO5 |
| R1 | D1 | GPIO27 |
| R2 | D2 | GPIO16 |
| R3 | D5 | GPIO17 |
| R4 | D6 | GPIO25 |

### 2. Kết nối LCD I2C

| Chân dây | ESP8266 | ESP32 |
|---|---|---|
| SCL | D1 | GPIO13 |
| SDA | D2 | GPIO26 |
| 5V | 5V | 5V |
| GND | GND | GND |

## Thư viện cần cài
Trước khi nạp chương trình, hãy cài các thư viện sau trong Arduino IDE:
- Keypad
- LiquidCrystal_I2C
- Wire

### Cách cài thư viện
1. Mở Arduino IDE.
2. Vào Tools -> Manage Libraries.
3. Tìm kiếm từ khóa "Keypad" và cài thư viện của tác giả Mark Stanley, Alexander Brevig.
4. Tìm kiếm "LiquidCrystal_I2C" và cài đặt.

## Hướng dẫn chạy chương trình
1. Mở Arduino IDE.
2. Tạo mới một sketch và đặt tên là Keypad_Lcd.ino.
3. Dán nội dung mã nguồn từ file Keypad_Lcd.ino vào sketch.
4. Chọn board phù hợp:
   - ESP8266 hoặc ESP32
5. Chọn đúng cổng COM đang kết nối với board.
6. Nhấn Upload để nạp chương trình vào board.

## Kết quả mong đợi
Sau khi nạp chương trình thành công:
- Khi bấm một phím trên keypad, ký tự tương ứng sẽ hiển thị trên LCD.
- Ký tự vừa bấm cũng sẽ được in ra Serial Monitor.
- Màn hình LCD sẽ hiển thị nội dung "Keypad" và ký tự vừa nhập.

## Lưu ý
- Nếu LCD không hiện chữ, hãy kiểm tra lại địa chỉ I2C (mặc định trong code là 0x27).
- Nếu không nhận được tín hiệu, hãy kiểm tra lại dây nối giữa keypad và board.
- Nếu dùng board ESP32, hãy chắc chắn đã chọn đúng board trong Arduino IDE.

## Mục đích học tập
Project này phù hợp để luyện tập các kỹ năng:
- Đọc tín hiệu từ keypad 4x4
- Giao tiếp với LCD1602 I2C
- Sử dụng thư viện Keypad trong Arduino
- Làm quen với ESP8266 / ESP32