#include <Joystick.h>

#define X1_AXIS A2
#define Y1_AXIS A3
#define HAT A1
#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4
#define BUTTON4 5
#define CONNECTION_LED 7

// debug over serial port with 115000 baud
//#define SER_DEBUG 1

const float threshold = 5;
// loop delay time in milliseconds: this will determine how often we send change events over the USB
// but also the maximum lag
const int delayTime = 5;
const bool reverseX = true;
const bool reverseY = true;
int x1, oldx1;
int y1, oldy1;
int hat, oldhat;
bool b1,b2,b3,b4;
bool oldb1, oldb2, oldb3, oldb4;
bool pluggedIn, oldPluggedIn;
bool hatActivated;
int hatValues[4] = {980, 191, 103, 71};

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,
  4, 4,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

void setup() {
#ifdef SER_DEBUG
  Serial.begin(115200);
#endif
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  pinMode(CONNECTION_LED, OUTPUT);
  pluggedIn = false;
  // read axis 1
  read1();
  // if there is a joystick connected read the cooliehat (the small "joystick")
  if (pluggedIn) {
    readCoolieHat();
  }
  // initialize Joystick library
  Joystick.begin();
  // trigger plugged-in check
  oldPluggedIn = !pluggedIn;
}

void read1() {
  oldx1 = x1;
  oldy1 = y1;
  x1 = analogRead(X1_AXIS);
  y1 = analogRead(Y1_AXIS);
  // a connected joystick has higher values even in extreme positions
  // that way we can detect a connected hardware
  pluggedIn = x1 > 5 && y1 > 5;
  if (reverseX) {
    x1 = 1023 - x1;
  }
  if (reverseY) {
    y1 = 1023 - y1;
  }
}

void readCoolieHat() {
  oldhat = hat;
  int h = analogRead(HAT);
  hat = -1;
  // the coolie hat uses four different resistors that give us different voltages
  for (int i = 0; i < 4; i++)
    if (abs(h-hatValues[i]) < 5) {
      hat = i * 90;
      break;
    }
}

void readButtons() {
  oldb1 = b1;
  oldb2 = b2;
  oldb3 = b3;
  oldb4 = b4;
  b1 = digitalRead(BUTTON1) == LOW;
  b2 = digitalRead(BUTTON2) == LOW;
  b3 = digitalRead(BUTTON3) == LOW;
  b4 = digitalRead(BUTTON4) == LOW;
}

void loop() {
  // sendState tracks the detected changes and controls whether we need to
  // update the USB joystick state
  bool sendState = false;
  read1();
  if (pluggedIn) {
    if (!oldPluggedIn) {
#ifdef SER_DEBUG
      Serial.println("plugged in");
#endif
      // start joystick emulation with auto-send-state feature turned off
      Joystick.begin(false);
      // switch connection LED on Micro on to show a joystick is connected
      digitalWrite(CONNECTION_LED, HIGH);
    }
    readButtons();
    if (b1 != oldb1 || b2 != oldb2 || b3 != oldb3 || b4 != oldb4) {
      Joystick.setButton(0, b1);
      Joystick.setButton(1, b2);
      Joystick.setButton(2, b3);
      Joystick.setButton(3, b4);
      sendState = true;
    }
    readCoolieHat();
    if (hat != oldhat) {
      Joystick.setHatSwitch(0, hat);
      sendState = true;
    }
    if (abs(x1-oldx1) > threshold || abs(y1-oldy1) > threshold) {
      Joystick.setXAxis(x1);
      Joystick.setYAxis(y1);
      sendState = true;
    }
    if (sendState) {
      Joystick.sendState();
#ifdef SER_DEBUG
      Serial.print("#1: ");
      Serial.print(b1 ? "1": "0");
      Serial.print(" #2: ");
      Serial.print(b2 ? "1": "0");
      Serial.print(" #3: ");
      Serial.print(b3 ? "1": "0");
      Serial.print(" #4: ");
      Serial.print(b4 ? "1": "0");
      Serial.print(" X#1: ");
      Serial.print(x1);
      Serial.print(" Y#1: ");
      Serial.print(y1);
      Serial.print(" HAT: ");
      Serial.println(hat);
#endif
    }
  } else if (oldPluggedIn) {
#ifdef SER_DEBUG
    Serial.println("unplugged");
#endif
    // stop the joystick emulation
    Joystick.end();
    // switch connection LED on Micro off to show there is no joystick connected
    digitalWrite(CONNECTION_LED, LOW);
  }
  oldPluggedIn = pluggedIn;
  delay(delayTime);
}
