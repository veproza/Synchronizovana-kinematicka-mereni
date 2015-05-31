int lastVal = LOW;
int val = LOW;
int ppsPin = 2;
unsigned long lastTime = micros();

void setup() {
  pinMode(ppsPin, INPUT);
  Serial.begin(115200);
}


void loop() {
  val = digitalRead(ppsPin);
  if(val == HIGH and lastVal == LOW) {
      Serial.println(micros());
//      lastTime = micros();
  }
  lastVal = val;
}
