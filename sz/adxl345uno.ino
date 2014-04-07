// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"
#include "I2Cdev.h"
#include "ADXL345.h"
#include "SerialCommand.h"

// class default I2C address is 0x53
// specific I2C addresses may be passed as a parameter here
// ALT low = 0x53 (default for SparkFun 6DOF board)
// ALT high = 0x1D
ADXL345 accel;
SerialCommand sCmd;

int16_t ax, ay, az;
float 	xg, yg, zg;

uint8_t rate, range;

char buffer[4];
int received;
int timer;

#define LED_PIN 13 // (Arduino is 13, Teensy is 6)
#define RED     8
#define YELLOW  9
#define GREEN   10
bool blinkState = false;
bool on         = false;
bool off        = true;



void setup() {

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(YELLOW, OUTPUT);
    pinMode(GREEN, OUTPUT);

    received = 0;
    buffer[received] = '\0';

    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    Serial.begin(9600);

    Serial.println("Initializing I2C devices...");
    
    digitalWrite(RED, on);
    digitalWrite(YELLOW, on);
    digitalWrite(GREEN, on);
    
    accel.initialize();

    do {
        Serial.println("Testing device connections...");
    } while (accel.testConnection() == 0);

    Serial.println("ADXL345 connection successful");
    
    accel.setRate(0xF);
    accel.setRange(0x3);
    delay(1000);

    digitalWrite(RED, off);
    digitalWrite(YELLOW, off);
    digitalWrite(GREEN, off);

    sCmd.setDefaultHandler(unrecognized);      // co jesli nie ma takiej komendy]
  	//sCmd.addCommand("#", comment);
  	sCmd.addCommand("h", help);
  	sCmd.addCommand("i2c", i2c_scan);
	sCmd.addCommand("start", startMeas);
  
	Serial.println("-------------------\n"); 
  	Serial.println("ZenMakers, @szkubicki\n"); 
  	Serial.println("Log: Engine console initialized. Ready when you are.");
  	Serial.println("Hint: Type h for help");
}



void help(){

 	Serial.println("Hint: i2c  : i2c scanner");
	Serial.println("      start: start measurements");
	Serial.println("\n");
}

void unrecognized(const char *command) {
	Serial.println("What? I don't know this command. ");
}




void i2c_scan() {
  byte error, address;
  int nDevices;

  Serial.println("Log: Scanning");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("Info: I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
        

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Info: Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("Info: No I2C devices found\n");
  else
    Serial.println("Log: I2C scan completed\n");
}


void startMeas() {
	sCmd.clearBuffer();
	//timer = 10;
	received = 0;
	while(Serial.available() == 0);
	delay(10);
	while (Serial.available()){
	 	buffer[received++] = Serial.read();
        buffer[received] = '\0';
        if (received >= (sizeof(buffer)-1))
        	timer = atoi(buffer);    
	}

	timer *= 50;
	
	for(int i=0; i<timer; i++){
		digitalWrite(RED, on);
	 	accel.getAcceleration(&ax, &ay, &az);

	 	xg = (float)ax / 32;
   		yg = (float)ay / 32;
     	zg = (float)az / 32;

	 	Serial.print(i);  Serial.print("\t");
     	Serial.print(xg); Serial.print("\t");
     	Serial.print(yg); Serial.print("\t");
     	Serial.print(zg); Serial.println("\t");
     	delay(1);
    }
    digitalWrite(RED, off);
}


void loop() {

    sCmd.readSerial();  
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    
}
