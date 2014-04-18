// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"
#include "I2Cdev.h"
#include "ADXL345.h"
#include "SD.h"
#include "SPI.h"

#include "Time.h"
#include "DS1307RTC.h"

// class default I2C address is 0x53
// specific I2C addresses may be passed as a parameter here
// ALT low = 0x53 (default for SparkFun 6DOF board)
// ALT high = 0x1D
ADXL345 accel;

int16_t ax, ay, az;
float   xg, yg, zg;

long i = 0;

//uint8_t rate, range;

const int chipSelect = 4;
String dataString = "";
String timeString = "";
String dayweek = "";

#define LED_PIN 13
#define RED     8
#define YELLOW  9
#define GREEN   10

bool blinkState = false;
bool on         = false;
bool off        = true;



void setup() {

    pinMode(LED_PIN, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(YELLOW, OUTPUT);
    pinMode(GREEN, OUTPUT);

    digitalWrite(RED, off);
    digitalWrite(YELLOW, off);
    digitalWrite(GREEN, off);

    Wire.begin();

    Serial.begin(57600);

    Serial.println("Initializing I2C devices...");       
    accel.initialize();
    do {
        Serial.println("Testing device connections...");
    } while (accel.testConnection() == 0);
    Serial.println("ADXL345 connection successful.");
    
    accel.setRate(0xF);
    accel.setRange(0x3);
    delay(2000);

    Serial.print("Initializing SD card...");
    pinMode(10, OUTPUT);
  
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present.");
      return;
    }
    Serial.println("SD card initialized.");

    delay(2000);


    setSyncProvider(RTC.get);   // the function to get the time from the RTC
  	if(timeStatus()!= timeSet) 
    	Serial.println("Unable to sync with the RTC");
  	else
    	Serial.println("RTC has set the system time"); 

    //setTime(13,06,30,18,4,14);
    //RTC.set(now());

    delay(2000);

}


void getTimeString(){


	timeString = "";
	timeString = String(year())   + String("-0") +
				 String(month())  + String("-") +
				 String(day())    + String("T") +
				 String(hour())   + String(":") +
				 String(minute()) + String(":") +
				 String(second()) + String(".000000Z ");

}


void Meas() {

    dataString = "";

    accel.getAcceleration(&ax, &ay, &az);
    
    xg = (float)ax / 32;
    yg = (float)ay / 32;
    zg = (float)az / 32;

    dataString = String(xg)  + String(" ") +
                 String(yg)  + String(" ") +
                 String(zg);
    
    delay(10);
}
  
void write2SD(String data){

  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if(dataFile) {
	dataFile.print(timeString);
    dataFile.println(dataString);
    dataFile.close();
    Serial.print(timeString);
    Serial.println(dataString);
  }  
  else {
    Serial.println("Error opening datalog.txt");
  } 
}


void loop() {

    Meas();
    getTimeString();
    //Serial.print(timeString);
    //Serial.println(dataString);
    //delay(500);
    write2SD(dataString);
    //blinkState = !blinkState;
    //digitalWrite(LED_PIN, blinkState);
    
}
