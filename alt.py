import serial
import numpy as np
from matplotlib import pyplot as plt
ser=serial.Serial("/dev/tty.usbserial-A7039GGP", 9600) 


x = None
y = None
z = None
ambient = None
proximity = None
 
plt.ion() # set plot to animated
 
ydata = [0] * 50
ax1=plt.axes()  
 
# make plot
line, = plt.plot(ydata)
plt.ylim([10,40])
 
# start data collection
while True:  
    

    data = ser.read().split(',')    
    
    try:
        x = float(data[0])
        y = float(data[1])
        z = float(data[2])
        ambient = int(data[3])
        proximity = int(data[4])
    except:
        print "does not parse"
    
        #print "%.2f %.2f %.2f %d %d" % (x,y,z,ambient,proximity)


        plt.ylim([-16,16])
        ydata.append(x)
    
   
        line.set_ydata(x)  # update the data
        plt.draw() # update the plot
    

