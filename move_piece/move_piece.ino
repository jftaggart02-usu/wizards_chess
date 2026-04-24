// Pinout
const int MX_DIR = 2;
const int MX_STEP = 3;
const int Y_BTN = 4;
const int X_BTN = 5;
const int MY_DIR = 6;
const int MY_STEP = 7;
const int EM_PIN = 9;

// Parameters
const int MOTOR_DELAY_US = 4000;   // The time it takes for the motors to complete one full step.

// Function prototypes
void fullStep(int duration, bool cw, int motor_id);
void fullStepMX(int duration, bool cw);
void fullStepMY(int duration, bool cw);
void fullStepBoth(int duration, bool cw_x, bool cw_y);
void zeroAxes();
int getDesiredX(int gridx, int stepx, int minx);
int getDesiredY(int gridy, int stepy, int miny);
void goToPositionX(int &x_pos, int desiredX);
void goToPositionY(int &y_pos, int desiredY);


void setup() {
  pinMode(MX_DIR, OUTPUT);
  pinMode(MX_STEP, OUTPUT);
  pinMode(MY_DIR, OUTPUT);
  pinMode(MY_STEP, OUTPUT);
  pinMode(X_BTN, INPUT_PULLUP);
  pinMode(Y_BTN, INPUT_PULLUP);
  pinMode(EM_PIN, OUTPUT);
  digitalWrite(MX_DIR, LOW);
  digitalWrite(MX_STEP, LOW);
  digitalWrite(MY_DIR, LOW);
  digitalWrite(MY_STEP, LOW);
  analogWrite(EM_PIN, 0);
  Serial.begin(9600);
  delay(2000);
}

void loop() {

  // Zero the axes using the limit switches
  zeroAxes();

  int x_pos = 0; // units: number of clockwise full steps from origin
  int y_pos = 0; // units: number of clockwise full steps from origin

  int minx = -1228;
  int maxx = 106;
  int stepx = (maxx-minx)/7;
  int miny = -1754;
  int maxy = -627;
  int stepy = (maxy-miny)/6;
  
  Serial.print("Step X = ");
  Serial.println(stepx);
  Serial.print("Step Y = ");
  Serial.println(stepy);

  // First, go to position (0,0)
  int desiredX = getDesiredX(0, stepx, minx);
  int desiredY = getDesiredY(0, stepy, miny);
  
  goToPositionX(x_pos, desiredX);
  goToPositionY(y_pos, desiredY);

  // Next, go to position (7, 6)
  analogWrite(EM_PIN, 60);
  desiredX = getDesiredX(7, stepx, minx);
  desiredY = getDesiredY(6, stepy, miny);

  goToPositionX(x_pos, desiredX);
  goToPositionY(y_pos, desiredY);
  analogWrite(EM_PIN, 0);

  while (true);

}

int getDesiredX(int gridx, int stepx, int minx) {
  return gridx*stepx + minx;
}

int getDesiredY(int gridy, int stepy, int miny) {
  return gridy*stepy + miny;
}

void goToPositionX(int &x_pos, int desiredX) {
  if (x_pos < desiredX) {
    while (x_pos < desiredX) {
      fullStepMX(MOTOR_DELAY_US, true);
      x_pos++;
    }
  }
  else if (x_pos > desiredX) {
    while (x_pos > desiredX) {
      fullStepMX(MOTOR_DELAY_US, false);
      x_pos--;
    }
  }
}

void goToPositionY(int &y_pos, int desiredY) {
  if (y_pos < desiredY) {
    while (y_pos < desiredY) {
      fullStepMY(MOTOR_DELAY_US, true);
      y_pos++;
    }
  }
  else if (y_pos > desiredY) {
    while (y_pos > desiredY) {
      fullStepMY(MOTOR_DELAY_US, false);
      y_pos--;
    }
  }
}

/*
Zero the linear slide rail axes using the limit switches.
*/
void zeroAxes() {
  int xBtn = digitalRead(X_BTN);
  while (xBtn == HIGH) {
    fullStepMX(MOTOR_DELAY_US, true);
    xBtn = digitalRead(X_BTN);
  }
  // Serial.println("X-axis zeroed.");
  for (int i = 0; i < 400; i++) {
    fullStepMX(MOTOR_DELAY_US, false);
  }

  int yBtn = digitalRead(Y_BTN);
  while (yBtn == HIGH) {
    fullStepMY(MOTOR_DELAY_US, true);
    yBtn = digitalRead(Y_BTN);
  }
  for (int i = 0; i < 20; i++) {
    fullStepMY(MOTOR_DELAY_US, false);
  }
  // Serial.println("Y-axis zeroed.");
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
