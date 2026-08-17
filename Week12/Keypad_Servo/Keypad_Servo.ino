/*
==========================================================
 Wemos D1 ESP32 + Servo Motor + Keypad 4x4
 Password : 159

 * hoặc # : Đóng cửa
 159       : Mở cửa
==========================================================
*/

#include <Keypad.h>
#include <ESP32Servo.h>

/*=========================================================
  KEYPAD
=========================================================*/

// Sơ đồ phím
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] =
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Theo sơ đồ đấu dây

// R1 -> IO13
// R2 -> IO12
// R3 -> IO14
// R4 -> IO27

byte rowPins[COLS] = {13, 12, 14, 27};

// C1 -> IO5
// C2 -> IO23
// C3 -> IO19
// C4 -> IO18

byte colPins[ROWS] = {5, 23, 19, 18};

Keypad keypad = Keypad(makeKeymap(keys),
                       rowPins,
                       colPins,
                       ROWS,
                       COLS);

/*=========================================================
  SERVO
=========================================================*/

#define SERVO_PIN 16

Servo myServo;

// Góc đóng
const int CLOSE_ANGLE = 0;

// Góc mở
const int OPEN_ANGLE = 180;

/*=========================================================
  PASSWORD
=========================================================*/

String password = "22206051";
int position = 0;

/*=========================================================
  SETUP
=========================================================*/

void setup()
{
  Serial.begin(115200);

  initServo();

  Serial.println();
  Serial.println("===== Door Lock =====");
  Serial.println("Password : 22206051");
  Serial.println("* hoặc # : Close");
}

/*=========================================================
  LOOP
=========================================================*/

void loop()
{
  char key = keypad.getKey();

  if (key)
  {
    Serial.print("Key: ");
    Serial.println(key);

    // Nhấn * hoặc # để đóng Servo
    if (key == '*' || key == '#')
    {
      downServo();
      position = 0;
    }

    // Kiểm tra mật khẩu
    else if (key == password[position])
    {
      position++;

      if (position == password.length())
      {
        upServo();
        position = 0;
      }
    }

    // Sai mật khẩu
    else
    {
      Serial.println("Wrong Password!");
      position = 0;
    }

    delay(100);
  }
}

/*=========================================================
  SERVO FUNCTIONS
=========================================================*/

void initServo()
{
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2400);

  downServo();

  Serial.println("Servo Ready");
}

void upServo()
{
  Serial.println("ACCESS GRANTED");
  Serial.println("OPEN DOOR");

  for (int angle = CLOSE_ANGLE; angle <= OPEN_ANGLE; angle++)
  {
    myServo.write(angle);
    delay(10);
  }
}

void downServo()
{
  Serial.println("CLOSE DOOR");

  for (int angle = OPEN_ANGLE; angle >= CLOSE_ANGLE; angle--)
  {
    myServo.write(angle);
    delay(10);
  }
}