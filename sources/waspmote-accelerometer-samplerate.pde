void setup()
{
  USB.println("START");
  ACC.ON();
  ACC.setSamplingRate(ACC_RATE_1000);
  USB.println(ARDUINO);
}

void loop()
{
  unsigned long t_start = millis();
  for(int i = 0; i < 1000; i++) {
    int acc = ACC.getZ();
  }
  unsigned long t_end = millis();
  USB.println(t_end - t_start);
}
