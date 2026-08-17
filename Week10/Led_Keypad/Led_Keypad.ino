#include <Keypad.h>

/* Keypad */

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] =
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] =
{
  13,
  12,
  14,
  27
};

byte colPins[COLS] =
{
  5,
  23,
  19,
  18
};

Keypad keypad =
Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

/* RGB LED */

#if defined(ESP32)

#define RED_LED_PIN     16    
#define GREEN_LED_PIN    4
#define BLUE_LED_PIN     2

#endif


//----------------------------------------------------
// setup
//----------------------------------------------------

void setup()
{
  Serial.begin(115200);

  initRGBLed();

  Serial.println();
  Serial.println("===== Keypad RGB Demo =====");
  Serial.println("1 : RED");
  Serial.println("2 : GREEN");
  Serial.println("3 : BLUE");
  Serial.println("4 : WHITE");
  Serial.println("5 : OFF");
  Serial.println("---------------------------");
}


//----------------------------------------------------
// loop
//----------------------------------------------------

String cmd = "";

void loop()
{
  char key = keypad.getKey();

  if ('0' <= key && key <= '9')
  {
    Serial.print(key);

    cmd += key;
  }

  else if (key == '*' || key == '#')
  {
    Serial.println();

    Serial.print("Command = ");
    Serial.println(cmd);

    if (cmd == "1")
    {
      onRGBLed(1,0,0);
      Serial.println("RED");
    }

    if (cmd == "2")
    {
      onRGBLed(0,1,0);
      Serial.println("GREEN");
    }

    if (cmd == "3")
    {
      onRGBLed(0,0,1);
      Serial.println("BLUE");
    }

    if (cmd == "4")
    {
      onRGBLed(1,1,1);
      Serial.println("WHITE");
    }

    if (cmd == "5")
    {
      onRGBLed(0,0,0);
      Serial.println("OFF");
    }

    cmd = "";
  }

  delay(100);
}


/*----------------------------------------------------
  RGB LED
----------------------------------------------------*/

void initRGBLed()
{
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  onRGBLed(0,0,0);
}


/*
    red   : 0 / 1
    green : 0 / 1
    blue  : 0 / 1
*/

void onRGBLed(int red, int green, int blue)
{
    digitalWrite(RED_LED_PIN, red);
    digitalWrite(GREEN_LED_PIN, green);
    digitalWrite(BLUE_LED_PIN, blue);
}