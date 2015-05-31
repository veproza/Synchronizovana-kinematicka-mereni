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
   - 
   Analog Joystick or two 10K potentiometers:
   GND to Arduino GND
   VCC to Arduino +5V
   X Pot to Arduino A0
   Y Pot to Arduino A1
   
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
#define JOYSTICK_X A0
#define JOYSTICK_Y A1

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
unsigned int data[2]; 

void setup()
{
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.setRetries(0,0);
  radio.setDataRate(RF24_1MBPS);
  radio.enableDynamicAck();
//  radio.setPayloadSize(sizeof(joystick));
  pinMode(7, OUTPUT);
}

unsigned int divider = 100;
unsigned long lastTime = millis() / divider;
unsigned long cntr = 0;
void loop()
{
  if(millis() / divider != lastTime) {
    Serial.println(millis());
    digitalWrite(7, HIGH);
    lastTime = millis() / divider;
    cntr += 1;
    data[0] = cntr; //micros();
    bool result = radio.writeFast(&data, sizeof(data), true);
    delay(1);
    digitalWrite(7, LOW);
  }

}

