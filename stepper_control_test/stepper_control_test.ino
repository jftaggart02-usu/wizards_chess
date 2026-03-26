const int M1_DIR = 2;
const int M1_STEP = 3;
const int CW_BTN = 4;
const int CCW_BTN = 5;
const int M2_DIR = 6;
const int M2_STEP = 7;

void fullStep(int duration, bool cw, int motor_id);
void fullStepM1(int duration, bool cw);
void fullStepM2(int duration, bool cw);

void setup() {
  pinMode(M1_DIR, OUTPUT);
  pinMode(M1_STEP, OUTPUT);
  pinMode(M2_DIR, OUTPUT);
  pinMode(M2_STEP, OUTPUT);
  pinMode(CW_BTN, INPUT_PULLUP);
  pinMode(CCW_BTN, INPUT_PULLUP);
  delay(2000);
}

void loop() {
  int cwButtonState = digitalRead(CW_BTN);
  int ccwButtonState = digitalRead(CCW_BTN);

  // Note: priority is given to the CW button

  if (cwButtonState == LOW) {  // CW Button pressed
    fullStepM1(1000, true);
    fullStepM2(1000, true);
  }
  else if (ccwButtonState == LOW) {  // CCW Button pressed
    fullStepM1(1000, false);
    fullStepM2(1000, false);
  }

}

/*
Parameters:
- duration: time to complete the full step.
- cw: if True, clockwise. Else, counterclockwise.
- motor_id: if 2, M2 is full-stepped. Else, M1 is full-stepped.
*/
void fullStep(int duration, bool cw, int motor_id) {
  int dir;
  int step;
  if (motor_id == 2){
    dir = M2_DIR;
    step = M2_STEP;
  }
  else {
    dir = M1_DIR;
    step = M1_STEP;
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

void fullStepM1(int duration, bool cw) {
  fullStep(duration, cw, 1);
}
void fullStepM2(int duration, bool cw) {
  fullStep(duration, cw, 2);
}
