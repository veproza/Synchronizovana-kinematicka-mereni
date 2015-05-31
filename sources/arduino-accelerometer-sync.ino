// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain

#include <Wire.h>
#include <TimerOne.h>

#define SYNC_PIN 12
#define SYNC_BIT 64 // 2^6 -> 0_00 0000

const int MPU=0x68;  // I2C address of the MPU-6050


int lastSecond = floor(millis() / 1000);
int second = floor(millis() / 1000);

volatile byte buf1[64];
volatile byte buf2[64];

volatile uint8_t counter1;

volatile uint8_t counter2;

volatile bool secondBuffer = false;
volatile bool overflow = false;

void setup(){
	pinMode(13, OUTPUT);
	pinMode(SYNC_PIN, INPUT);
	Serial.begin(115200);
        Serial.print("Hello");
	Wire.begin();
        Serial.print("A");
	Wire.beginTransmission(MPU);
        Serial.print("B");
	Wire.write(0x6B);  // PWR_MGMT_1 register
	Wire.write(0);     // set to zero (wakes up the MPU-6050)
        Serial.print("C");
	Wire.endTransmission(true);
        Serial.print("D");
	Timer1.initialize(1000);
	Timer1.attachInterrupt(readData);
        Serial.print("E");
}


void loop() {
	second = floor(millis() / 1000);
	if(overflow) {
		noInterrupts();
		digitalWrite(13, HIGH);
		while(1) {};
	}

	if(counter1 >= 64 || counter2 >= 64) {
		byte toSend[64];
		noInterrupts();
		if(counter1 >= 64) {
			memcpy((char*)toSend, (char*)buf1, 64);
			counter1 = 0;
		} else {
			memcpy((char*)toSend, (char*)buf2, 64);
			counter2 = 0;
		}
		interrupts();
		Serial.write(toSend,64);
	}
	lastSecond = second;
}

void readData(){
	interrupts();

	Wire.beginTransmission(MPU);
	Wire.write(0x3F);  // starting with register 0x3B (ACCEL_XOUT_H)
	Wire.endTransmission(false);
	Wire.requestFrom(MPU,2,true);
	uint8_t acc_msb = Wire.read();
	uint8_t acc_lsb = Wire.read();
	bool syncPulse = digitalRead(SYNC_PIN);
	noInterrupts();
	uint16_t time = micros() % 10000;
	uint8_t time_msb = time >> 8;
	uint8_t time_lsb = time;

	if (syncPulse) {
		time_msb = time_msb | SYNC_BIT;
	}


	if(secondBuffer == false) {
		if(counter1 == 64) {
			overflow = true;
		} else {
			buf1[counter1] = acc_msb;
			buf1[counter1 + 1] = acc_lsb;
			buf1[counter1 + 2] = time_msb;
			buf1[counter1 + 3] = time_lsb;
			counter1 += 4;
			if(counter1 == 64) {
				secondBuffer = true;
			}
		}
	} else {
		if(counter2 == 64) {
			overflow = true;
		} else {
			buf2[counter2] = acc_msb;
			buf2[counter2 + 1] = acc_lsb;
			buf2[counter2 + 2] = time_msb;
			buf2[counter2 + 3] = time_lsb;
			counter2 += 4;
			if(counter2 == 64) {
				secondBuffer = false;
			}
		}
	}
}
