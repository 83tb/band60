##====================================
##
##  ZenMakers, @szkubicki
##
##====================================
##

import serial
import time
import sys

## Boolean variable that will represent 
## whether or not the arduino is connected
connected = False
start = False
loop = True
status = False

## establish connection to the serial port that your arduino 
## is connected to.

locations=['/dev/tty.usbmodem1421']

for device in locations:
    try:
        print "Trying...",device
        ser = serial.Serial(device, 9600)
        break
    except:
        print "Failed to connect on",device

## loop until the arduino tells us it is ready
while not connected:
    serin = ser.read()
    connected = True


while ser.inWaiting():
    x=ser.readline()
    print x
time.sleep(2)

while ser.inWaiting():
    x=ser.readline()
    print x


while True:
    str = raw_input("Enter command: ");
    
    if str == "koniec":
        ser.close()
        time.sleep(1)
        raise SystemExit
    
    if str == "i2c":
        ser.write("i2c\n")
        time.sleep(.1)
        while ser.inWaiting():
            x=ser.readline()
            print x
            time.sleep(.05)

    if str == "start":
        status = False
        fileName = raw_input("File name: ");
        measTime = raw_input("Meas time [s]: ");
        ser.write("start\n")
        time.sleep(1)
        ser.write(measTime + "\r\n")
        time.sleep(1)
        
        text_file = open(fileName, 'w')
        
        while ser.inWaiting():
            x=ser.readline()
            print x 
            text_file.write(x)
            time.sleep(.01)

        text_file.flush()
        text_file.close()

