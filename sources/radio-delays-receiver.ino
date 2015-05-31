/*
 - CONNECTIONS: nRF24L01 Modules See:
 http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED

 - V1.00 11/26/13
   Based on examples at http://www.bajdi.com/
   Questions: terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
unsigned long data[1];
unsigned long lastTime = NULL;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  radio.setAutoAck(true);
  lastTime = micros();
  pinMode(7, INPUT);
}


int lastState = LOW;
int lastIrqState = LOW;
unsigned int bad = 0;
unsigned int received = 0;
unsigned long lastIrqTime = micros();
void loop()
{
  int state = digitalRead(7);
  int irqState = digitalRead(8);
  if(state == HIGH && lastState == LOW) {
    lastTime = micros();
    bad = bad + 1;
  }
  if(irqState == HIGH && lastIrqState == LOW) {
    unsigned long time = micros();
    Serial.print(time - lastIrqTime);
    Serial.print("\t");
    Serial.println(time - lastTime);
    lastIrqTime = time;
  }
  lastIrqState = irqState;
  lastState = state;
  int lost = 0;
  if ( radio.available() )
  {
    unsigned long time = micros();
    // Read the data payload until we've received everything
    bool done = false;
    radio.read( data, sizeof(data) );
//    Serial.println(time - lastTime);
  }

}
