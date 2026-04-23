// Pinout
const int MX_DIR = 2;
const int MX_STEP = 3;
const int Y_BTN = 4;
const int X_BTN = 5;
const int MY_DIR = 6;
const int MY_STEP = 7;
const int JS_RX = A0;
const int JS_RY = A1;

// Parameters
const int JS_DEADZONE = 100;  // Joystick deadzone. For reference, joystick values are from 0 to 1023, and the middle value is 512.

// Function prototypes
void fullStep(int duration, bool cw, int motor_id);
void fullStepMX(int duration, bool cw);
void fullStepMY(int duration, bool cw);
void zeroAxes();

void setup() {
  pinMode(M1_DIR, OUTPUT);
  pinMode(M1_STEP, OUTPUT);
  pinMode(M2_DIR, OUTPUT);
  pinMode(M2_STEP, OUTPUT);
  pinMode(CW_BTN, INPUT_PULLUP);
  pinMode(CCW_BTN, INPUT_PULLUP);
  Serial.begin(9600);
  delay(2000);
}

void loop() {

  // Zero the axes using the limit switches
  zeroAxes();

  int x_pos = 0; // units: number of clockwise full steps from origin
  int y_pos = 0; // units: number of clockwise full steps from origin

  // Control the stepper motors while printing out the current x-y position
  while (true) {
    // Read the joystick position
    int js_x = analogRead(JS_RX);  // 0-1023. Center is 512
    int js_y = analogRead(JS_RY);

    // Determine how we should move the motors based on joystick position
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
    if (move_x > 0) {
      fullStepMX(1000, true);
      x_pos += 1;
    }
    else if (move_x < 0) {
      fullStepMX(1000, false);
      x_pos -= 1;
    }
    if (move_y > 0) {
      fullStepMY(1000, true);
      y_pos += 1;
    }
    else if (move_y < 0) {
      fullStepMY(1000, false);
      y_pos -= 1;
    }

    // Print the current axis positions
    Serial.print("(x,y): (");
    Serial.print(x_pos);
    Serial.print(", ");
    Serial.print(y_pos);
    Serial.println(")");

  }

}

/*
Zero the linear slide rail axes using the limit switches.
*/
void zeroAxes() {
  int xBtn = digitalRead(X_BTN);
  while (xBtn == LOW) {
    fullStepMX(1000);
    xBtn = digitalRead(X_BTN);
  }
  Serial.println("X-axis zeroed.");

  int yBtn = digitalRead(Y_BTN);
  while (yBtn == LOW) {
    fullStepMY(1000);
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
