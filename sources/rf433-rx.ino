void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  Serial.begin(115200);
}

unsigned long lastValidTime = micros();
unsigned long lastEdgeTime = micros();
unsigned long referenceTime = micros();
unsigned long now = micros();

int lastReferenceState = LOW;
int lastState = LOW;
int state = LOW;
void loop() {
  now = micros();
  int referenceState = digitalRead(12);
  if(referenceState == HIGH && lastReferenceState == LOW) {
    referenceTime = now;
  }
  lastReferenceState = referenceState;

  
  state = digitalRead(2);
  if(state == HIGH && lastState == LOW) {
    if(now - lastEdgeTime > 8000) {
      Serial.print(now);
      Serial.print("\t");
      Serial.print(referenceTime);
      Serial.print("\t");
      Serial.print(now - lastValidTime);
      Serial.print("\t");
      Serial.print(now - lastEdgeTime);
      Serial.print("\t");
      Serial.println(now - referenceTime);
      lastValidTime = now;
    }
    lastEdgeTime = now;
  }
  digitalWrite(13, state);
}
