
#include <SerialCommand.h>
#include <Wire.h>

#include "I2Cdev.h"
#include "ADXL345.h"

// VNCL PARAMETERS

#define VCNL4000_ADDRESS 0x13
#define VCNL4000_COMMAND 0x80
#define VCNL4000_PRODUCTID 0x81
#define VCNL4000_IRLED 0x83
#define VCNL4000_AMBIENTPARAMETER 0x84
#define VCNL4000_AMBIENTDATA 0x85
#define VCNL4000_PROXIMITYDATA 0x87
#define VCNL4000_SIGNALFREQ 0x89
#define VCNL4000_PROXINITYADJUST 0x8A

#define VCNL4000_3M125 0
#define VCNL4000_1M5625 1
#define VCNL4000_781K25 2
#define VCNL4000_390K625 3

#define VCNL4000_MEASUREAMBIENT 0x10
#define VCNL4000_MEASUREPROXIMITY 0x08
#define VCNL4000_AMBIENTREADY 0x40
#define VCNL4000_PROXIMITYREADY 0x20

// EOF VNCL PARAMETERS

int16_t ax, ay, az;
float xg, yg, zg;

uint8_t rate, range;




ADXL345 accel;
SerialCommand sCmd;     // obiekt komunikacji Serial

void setup() {
Serial.println("TEST");
  
  Wire.begin();
  

  Serial.begin(9600);


  // VCNL INIT
  /*
  uint8_t rev = read8(VCNL4000_PRODUCTID);
  
   if ((rev & 0xF0) != 0x10) {
    Serial.println("Ambient and Proximity sensors not found :(");
    while (1);
   }

  Serial.print("Proximity measurement frequency = ");
  uint8_t freq = read8(VCNL4000_SIGNALFREQ);
  
  if (freq == VCNL4000_3M125) Serial.println("3.125 MHz");
  if (freq == VCNL4000_1M5625) Serial.println("1.5625 MHz");
  if (freq == VCNL4000_781K25) Serial.println("781.25 KHz");
  if (freq == VCNL4000_390K625) Serial.println("390.625 KHz");


  
  write8(VCNL4000_PROXINITYADJUST, 0x81);
  Serial.print("Proximity adjustment register = ");
  Serial.println(read8(VCNL4000_PROXINITYADJUST), HEX);

  */
  // EOF VCNL INIT

  // ACCEL INIT

  accel.initialize(); 
  do {
        // Serial.println("Testing device connections...");
    } while (accel.testConnection() == 0);

  // Serial.println("ADXL345 connection successful");
    
  accel.setRate(0xF);
  accel.setRange(0x3);
  delay(1000);
  
 // EOF ACCEL INIT


  sCmd.setDefaultHandler(unrecognized);      // co jesli nie ma takiej komendy]
  sCmd.addCommand("#", comment);
  sCmd.addCommand("h", help);
  sCmd.addCommand("i2c", i2c_scan);

  sCmd.addCommand("ra", read_accel);
  sCmd.addCommand("am", read_proximity);
  

  // Serial.println("-------------------\n"); 
  // Serial.println("ZenMakers, @83TB\n"); 
  // Serial.println("Log: Engine console initialized. Ready when you are.");
  // Serial.println("Hint: Type h for help");
  
}


void help(){

 Serial.println("Hint: i2c: i2c scanner");
 Serial.println("\n");

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

void loop() {
  sCmd.readSerial();     // Przetwarzanie, to wszystko co dzieje sie w petli
  accel.getAcceleration(&ax, &ay, &az);

  xg = (ax * 31.2)/1000;
  yg = (ay * 31.2)/1000;
  zg = (az * 31.2)/1000;
  Serial.print(xg);
  Serial.print(",");
  Serial.print(yg);
  Serial.print(",");
  Serial.print(zg);
  Serial.println(",0,0");
  delay(100);


  

}



void read_accel() {

    accel.getAcceleration(&ax, &ay, &az);

    xg = (ax * 31.2)/1000;
    yg = (ay * 31.2)/1000;
    zg = (az * 31.2)/1000;


    Serial.println("accel:\t");
    Serial.print("Os X: "); Serial.print(xg); Serial.print(" g\t\t");
    Serial.print("Os Y: "); Serial.print(yg); Serial.print(" g\t\t");
    Serial.print("Os Z: "); Serial.print(zg); Serial.println(" g\t\t");
    delay(250);
    

}


void read_proximity() {

  write8(VCNL4000_COMMAND, VCNL4000_MEASUREAMBIENT | VCNL4000_MEASUREPROXIMITY); 
    while (1) {
    uint8_t result = read8(VCNL4000_COMMAND);
    //Serial.print("Ready = 0x"); Serial.println(result, HEX);
    if ((result & VCNL4000_AMBIENTREADY)&&(result & VCNL4000_PROXIMITYREADY)) {

      Serial.print("Ambient = ");
      Serial.print(read16(VCNL4000_AMBIENTDATA));
      Serial.print("\t\tProximity = ");
      Serial.println(read16(VCNL4000_PROXIMITYDATA));
      break;
    }
    delay(10);
  }
  
   delay(100);
 }

// Read 1 byte from the VCNL4000 at 'address'
uint8_t read8(uint8_t address)
{
  uint8_t data;

  Wire.beginTransmission(VCNL4000_ADDRESS);
#if ARDUINO >= 100
  Wire.write(address);
#else
  Wire.send(address);
#endif
  Wire.endTransmission();

  delayMicroseconds(170);  // delay required

  Wire.requestFrom(VCNL4000_ADDRESS, 1);
  while(!Wire.available());

#if ARDUINO >= 100
  return Wire.read();
#else
  return Wire.receive();
#endif

}


void comment() {
  
}


// nie rozpoznano komendy
void unrecognized(const char *command) {
  Serial.println("What? I don't know this command. ");
}


uint16_t readProximity() {
  write8(VCNL4000_COMMAND, VCNL4000_MEASUREPROXIMITY);
  while (1) {
    uint8_t result = read8(VCNL4000_COMMAND);
    //Serial.print("Ready = 0x"); Serial.println(result, HEX);
    if (result & VCNL4000_PROXIMITYREADY) {
      return read16(VCNL4000_PROXIMITYDATA);
    }
    delay(1);
  }
}


// Read 2 byte from the VCNL4000 at 'address'
uint16_t read16(uint8_t address)
{
  uint16_t data;

  Wire.beginTransmission(VCNL4000_ADDRESS);
#if ARDUINO >= 100
  Wire.write(address);
#else
  Wire.send(address);
#endif
  Wire.endTransmission();

  Wire.requestFrom(VCNL4000_ADDRESS, 2);
  while(!Wire.available());
#if ARDUINO >= 100
  data = Wire.read();
  data <<= 8;
  while(!Wire.available());
  data |= Wire.read();
#else
  data = Wire.receive();
  data <<= 8;
  while(!Wire.available());
  data |= Wire.receive();
#endif
  
  return data;
}

// write 1 byte
void write8(uint8_t address, uint8_t data)
{
  Wire.beginTransmission(VCNL4000_ADDRESS);
#if ARDUINO >= 100
  Wire.write(address);
  Wire.write(data);  
#else
  Wire.send(address);
  Wire.send(data);  
#endif
  Wire.endTransmission();
}



