const int BTN_PIN = 8;
const int EM_PIN = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(EM_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int btnState = digitalRead(BTN_PIN);
  if (btnState == LOW) {  // button pressed
    analogWrite(EM_PIN, 53);  // 5V
  }
  else {
    analogWrite(EM_PIN, 0);
  }
}
