/* YourDuinoStarter Example: nRF24L01 Transmit Joystick values
 - WHAT IT DOES: Reads Analog values on A0, A1 and transmits
   them over a nRF24L01 Radio Link to another transceiver.
 - SEE the comments after "//" on each line below
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

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
#define PAYLOAD_SIZE 96
byte data[PAYLOAD_SIZE];
void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.setRetries(5,2);
  radio.setDataRate(RF24_2MBPS);
  pinMode(7, OUTPUT);
}//--(end setup )---

unsigned int divider = 100;
unsigned long lastTime = millis() / divider;
uint8_t cntr = 0;
void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  lastTime = millis() / divider;
  cntr += 1;
  for(uint8_t i = 0; i < PAYLOAD_SIZE; ++i) {
    data[i] = cntr + i;
  }
  byte data1[32];
  byte data2[32];
  byte data3[32];
  for(uint8_t i = 0; i < PAYLOAD_SIZE; ++i) {
    if(i < 32) {
      data1[i] = data[i];
    } else if (i < 64) {
      data2[i - 32] = data[i];
    } else {
      data3[i - 64] = data[i];
    }
  }
  unsigned long t0 = micros();
  radio.writeFast(&data1, 32);
  radio.writeFast(&data2, 32);
  radio.writeFast(&data3, 32);
  bool result = radio.txStandBy(1000);
  unsigned long t1 = micros();
  Serial.print(result);
  Serial.print("\t");
  Serial.println(t1 - t0);

}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

//NONE
//*********( THE END )***********


