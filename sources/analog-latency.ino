void setup() {
  // put your setup code here, to run once:
  pinMode(12, INPUT);
  Serial.begin(115200);
}

int lastAnalogState = LOW;
int lastDigitalState = LOW;
unsigned long digitalRise = 0;
unsigned long analogRise = 0;
void loop() {
  // put your main code here, to run repeatedly:
  int digitalState = digitalRead(12);
  int analogState = analogRead(A1) > 512;
  if(digitalState == HIGH && lastDigitalState == LOW) {
    digitalRise = micros();
  }
  if(analogState == true && lastAnalogState == false) {
    analogRise = micros();
    Serial.println(analogRise - digitalRise);
  }
  lastDigitalState = digitalState;
  lastAnalogState = analogState;
}
