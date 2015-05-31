void setup()
{

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  Serial.begin(115200);
}

int state1;
int state2;
long time1 = micros();
long time2 = micros();
unsigned long c = 0;
unsigned long cycles1;
unsigned long cycles2;
void loop()
{
  while(1) {
    c++;
    int s1 = digitalRead(2);
    int s2 = digitalRead(3);
    long t = micros();
    if (s1 == HIGH && state1 == LOW) {
      time1 = t;
      cycles1 = c;
    }
    state1 = s1;
    if (s2 == HIGH && state2 == LOW) {
      time2 = t;
      cycles2 = c;
    }
    if (s2 == LOW && state2 == HIGH) {
      Serial.print(time1 - time2);
      Serial.print("\t");
      Serial.print(time1);
      Serial.print("\t");
      Serial.print(time2);
      Serial.print("\t");
      Serial.print(cycles1);
      Serial.print("\t");
      Serial.print(cycles2);
      Serial.print("\t");
      Serial.print(cycles1 - cycles2);
      Serial.println();
    }
    state2 = s2;
  }
}
