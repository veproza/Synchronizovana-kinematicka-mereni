/* YourDuinoStarter Example: nRF24L01 Receive Joystick values

 - WHAT IT DOES: Receives data from another transceiver with
   2 Analog values from a Joystick or 2 Potentiometers
   Displays received values on Serial Monitor
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
#define PAYLOAD_SIZE 32
byte data[PAYLOAD_SIZE];
unsigned long lastTime = NULL;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.setDataRate(RF24_2MBPS);
  radio.startListening();
  radio.setAutoAck(true);
  radio.setPayloadSize(PAYLOAD_SIZE);
  lastTime = micros();
//  pinMode(3, INPUT);
}//--(end setup )---


int lastState = LOW;
unsigned int bad = 0;
unsigned int received = 0;
void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
//  int state = digitalRead(3);
//  if(state == HIGH && lastState == LOW) {
//    lastTime = micros();
//    bad = bad + 1;
//  }
//  lastState = state;
  int lost = 0;
  if ( radio.available() )
  {
    unsigned long time = micros();
    // Read the data payload until we've received everything
    bool done = false;
    radio.read( data, PAYLOAD_SIZE);
    Serial.print(data[0]);
    if(0 == received % 3) {
      Serial.println();
    } else {    
      Serial.print("\t");
    }
    unsigned long diff = time - lastTime;
    unsigned long lost = (diff / 100000) - 1;
    received++;
    if(lost && false) {
      bad += lost;
      Serial.println();
      Serial.print("Lost: ");
      Serial.println(bad);
      Serial.print("Received: ");
      Serial.println(received);
      Serial.print("Ratio: ");
      Serial.println(100.0 * bad / (bad + received));
    }
    lastTime = time;
  }

}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

//NONE
//*********( THE END )***********


