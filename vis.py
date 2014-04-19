import serial
import sys
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
ser=serial.Serial("/dev/tty.usbserial-A7039GGP", 9600)  

last_datapoints = deque(maxlen=1000)

class DataPoint:
    x = None
    y = None
    z = None
    ambient = None
    proximity = None

    def __init__(self,x,y,z,a,p):
        self.x = x
        self.y = y
        self.z = z
        self.ambient = a
        self.proximity = p

def parseData(line):
    data = line.split(',')
    data = [d.strip() for d in data if len(d.strip()) > 0]
    if len(data) != 5:
        print >> sys.stderr, "WONT PARSE %s" % line
        return
    try:
        x, y, z, ambient, proximity = float(data[0]), float(data[1]), float(data[2]), int(data[3]), int(data[4])
    except:
        print >> sys.stderr, "WONT PARSE %s" % line
        return
    last_datapoints.append(DataPoint(x,y,z,ambient,proximity))
    print "%.2f %.2f %.2f %d %d" % (x,y,z,ambient,proximity)

def receiving(ser):
    buffer = ''
    while True:
        buffer = buffer + ser.read(ser.inWaiting())
        if '\n' in buffer:
            lines = buffer.split('\n')
            for line in lines[:-1]:
                parseData(line)
            buffer = lines[-1]

if __name__=="__main__":
    receiving(ser)

    