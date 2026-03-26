const int dirPin = 2;
const int stepPin = 3;

void fullStep(int delay);

void setup() {
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  delay(2000);
}

void loop() {
  // clockwise
  for (int x = 0; x < 400; x++) {
    fullStep(1000, true);
  }
  delay(1000);

  //counter clockwise
  digitalWrite(dirPin, LOW);
  for (int x = 0; x < 400; x++) {
    fullStep(1000, false);
  }
  delay(1000);
}

/*
Parameters:
- duration: time to complete the full step.
- cw: if True, clockwise. Else, counterclockwise.
*/
void fullStep(int duration, bool cw) {
    if (cw){
      digitalWrite(dirPin, HIGH);
    }
    else {
      digitalWrite(dirPin, LOW);
    }
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(duration/2);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(duration/2);
}
