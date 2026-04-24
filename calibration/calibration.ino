// Pinout
const int MX_DIR = 2;
const int MX_STEP = 3;
const int Y_BTN = 4;
const int X_BTN = 5;
const int MY_DIR = 6;
const int MY_STEP = 7;
const int JS_RX = A0;
const int JS_RY = A1;
const int JS_BTN = 8;
const int EM_PIN = 9;

// Parameters
const int JS_DEADZONE = 200;  // Joystick deadzone. For reference, joystick values are from 0 to 1023, and the middle value is 512.
const int MOTOR_DELAY_US = 4000;   // The time it takes for the motors to complete one full step.

// Function prototypes
void fullStep(int duration, bool cw, int motor_id);
void fullStepMX(int duration, bool cw);
void fullStepMY(int duration, bool cw);
void fullStepBoth(int duration, bool cw_x, bool cw_y);
void zeroAxes();
int readFilteredX(int pin);
int readFilteredY(int pin); 

void setup() {
  pinMode(MX_DIR, OUTPUT);
  pinMode(MX_STEP, OUTPUT);
  pinMode(MY_DIR, OUTPUT);
  pinMode(MY_STEP, OUTPUT);
  pinMode(X_BTN, INPUT_PULLUP);
  pinMode(Y_BTN, INPUT_PULLUP);
  pinMode(JS_BTN, INPUT_PULLUP);
  pinMode(EM_PIN, OUTPUT);
  digitalWrite(MX_DIR, LOW);
  digitalWrite(MX_STEP, LOW);
  digitalWrite(MY_DIR, LOW);
  digitalWrite(MY_STEP, LOW);
  // Serial.begin(9600);
  delay(2000);
}

void loop() {

  // Zero the axes using the limit switches
  // zeroAxes();

  int x_pos = 0; // units: number of clockwise full steps from origin
  int y_pos = 0; // units: number of clockwise full steps from origin

  // Control the stepper motors while printing out the current x-y position
  while (true) {
    // Activate the electromagnet if the joystick button is pressed
    int js_btn = digitalRead(JS_BTN);
    if (js_btn == LOW) {  // button pressed
      analogWrite(EM_PIN, 53);  // 5 Volts (electromagnet supply voltage is 24 volts)
    }
    else {
      analogWrite(EM_PIN, 0);
    }

    // Determine how we should move the motors based on joystick position
    int js_x = readFilteredX(JS_RX);  // 0-1023. Center is 512
    int js_y = readFilteredY(JS_RY);
    int move_x = 0;  // 0 = don't move. Positive = move CW. Negative = move CCW
    if (js_x > 512 + JS_DEADZONE/2) {
      move_x = 1;
    }
    else if (js_x < 512 - JS_DEADZONE/2) {
      move_x = -1;
    }
    int move_y = 0;  // 0 = don't move. Positive = move CW. Negative = move CCW
    if (js_y > 512 + JS_DEADZONE/2) {
      move_y = 1;
    }
    else if (js_y < 512 - JS_DEADZONE/2) {
      move_y = -1;
    }

    // Move the motors and update the axis positions
    if (move_y && move_x) {
      fullStepBoth(MOTOR_DELAY_US, move_x > 0, move_y > 0);
    }
    else if (move_x) {
      fullStepMX(MOTOR_DELAY_US, move_x > 0);
      if (move_x > 0) {
        x_pos += 1;
      }
      else {
        x_pos -= 1;
      }
    }
    else if (move_y) {
      fullStepMY(MOTOR_DELAY_US, move_y > 0);
      if (move_y > 0) {
        y_pos += 1;
      }
      else {
        y_pos -= 1;
      }
    }
    else {
      delayMicroseconds(MOTOR_DELAY_US);
    }

    // Print the current axis positions
    // Serial.print("(x,y): (");
    // Serial.print(x_pos);
    // Serial.print(", ");
    // Serial.print(y_pos);
    // Serial.println(")");

  }

}

/*
Zero the linear slide rail axes using the limit switches.
*/
void zeroAxes() {
  int xBtn = digitalRead(X_BTN);
  while (xBtn == LOW) {
    fullStepMX(1000, true);
    xBtn = digitalRead(X_BTN);
  }
  Serial.println("X-axis zeroed.");

  int yBtn = digitalRead(Y_BTN);
  while (yBtn == LOW) {
    fullStepMY(1000, true);
    yBtn = digitalRead(Y_BTN);
  }
  Serial.println("Y-axis zeroed.");
}

/*
Make the motor do a full step.

Parameters:
- duration: time to complete the full step in microseconds.
- cw: if True, clockwise. Else, counterclockwise.
- motor_id: if 2, MY is full-stepped. Else, MX is full-stepped.
*/
void fullStep(int duration, bool cw, int motor_id) {
  int dir;
  int step;
  if (motor_id == 2){
    dir = MY_DIR;
    step = MY_STEP;
  }
  else {
    dir = MX_DIR;
    step = MX_STEP;
  }
  if (cw){
    digitalWrite(dir, HIGH);
  }
  else {
    digitalWrite(dir, LOW);
  }
  digitalWrite(step, HIGH);
  delayMicroseconds(duration/2);
  digitalWrite(step, LOW);
  delayMicroseconds(duration/2);
}

void fullStepMX(int duration, bool cw) {
  fullStep(duration, cw, 1);
}
void fullStepMY(int duration, bool cw) {
  fullStep(duration, cw, 2);
}
void fullStepBoth(int duration, bool cw_x, bool cw_y) {
  if (cw_x){
    digitalWrite(MX_DIR, HIGH);
  }
  else {
    digitalWrite(MX_DIR, LOW);
  }
  if (cw_y) {
    digitalWrite(MY_DIR, HIGH);
  }
  else {
    digitalWrite(MY_DIR, LOW);
  }
  digitalWrite(MX_STEP, HIGH);
  digitalWrite(MY_STEP, HIGH);
  delayMicroseconds(duration/2);
  digitalWrite(MX_STEP, LOW);
  digitalWrite(MY_STEP, LOW);
  delayMicroseconds(duration/2);
}

int readFilteredX(int pin) {
  static int val = 512;
  int raw = analogRead(pin);
  val = (4 * val + raw) / 5;  // better integer filter
  return val;
}
int readFilteredY(int pin) {
  static int val = 512;
  int raw = analogRead(pin);
  val = (4 * val + raw) / 5;  // better integer filter
  return val;
}
