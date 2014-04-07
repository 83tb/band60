// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"
#include "I2Cdev.h"
#include "ADXL345.h"

// class default I2C address is 0x53
// specific I2C addresses may be passed as a parameter here
// ALT low = 0x53 (default for SparkFun 6DOF board)
// ALT high = 0x1D
ADXL345 accel;

int16_t ax, ay, az;
float 	xg, yg, zg;

long i;

uint8_t rate, range;

char buffer[4];
int received;
int timer;

#define LED_PIN 13 // (Arduino is 13, Teensy is 6)
bool blinkState = false;
bool on         = false;
bool off        = true;



void setup() {

    pinMode(LED_PIN, OUTPUT);

    Wire.begin();

    Serial.begin(9600);

    Serial.println("Initializing I2C devices...");       
    accel.initialize();
    do {
        Serial.println("Testing device connections...");
    } while (accel.testConnection() == 0);
    Serial.println("ADXL345 connection successful");
    
    accel.setRate(0xF);
    accel.setRange(0x3);
    delay(1000);

    i = 0;
}

void Meas() {

		accel.getAcceleration(&ax, &ay, &az);

	 	xg = (float)ax / 32;
   	yg = (float)ay / 32;
    zg = (float)az / 32;

	 	Serial.print(i++); Serial.print("\t");
   	Serial.print(xg);  Serial.print("\t");
  	Serial.print(yg);  Serial.print("\t");
  	Serial.print(zg);  Serial.println("\t");
   	delay(10);
}
  


void loop() {

    Meas();
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    
}
