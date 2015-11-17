############################
# Author: Nicholas Palermo
# Date: nov, 2016
# intended for use with an arduino and ir sensors.
###########################

#!/usr/bin/python

import serial
import datetime 
import time as t
import os

class Arduino():
    def __init__(self):
        self.sensorCalibrated=False
        self.connected=False
        self.ser=False
        self.connect()
        
    def connect(self):
        try:
            self.ser=serial.Serial("/dev/ttyACM0",9600,timeout=0)                  
        except:
            self.ser=serial.Serial('/dev/ttyACM1',9600,timeout=0)   
        t.sleep(2)
        print "Arduino connected"
        self.connected=True

    def write(self,value):
        self.ser.write(value)
        t.sleep(.5)   
       
    def read(self):
        return self.ser.read(self.ser.inWaiting())


def current_time():
    return t.strftime("Time: %H:%M:%S")


def main():
    arduino = Arduino()
    running = True

    arduino.write("1")      # calibrate the arduinos
    arduino.write("2")      # start recording
    while running:
        print current_time()
        if not os.path.exists("bee_boxes_data/"):
            os.makedirs("bee_boxes_data/")
            fileTimeName1=(t.strftime('%Y_%m_%d.txt'))
            filename1= "bee_boxes_data/" + fileTimeName1
            print "\n..................recording..................."
        
            count=[0,0,0,0,0,0]
            while task==2:
                f=open(filename1,"a")
                if arduino.ser.inWaiting()>0:
                    item=arduino.read()
                    if item != " " or item != "":

                        try:
                            count[int(item)] += 1
                            f.write("sensor ")
                            f.write(str(item))
                            f.write("\tdate_time: ")
                            f.write(str(t.localtime()))
                            f.write("\tcount:")
                            f.write(str(count[int(item)]))
                            f.write("\n")
                            print str(item) + "\tCount: "+ str(count[int(item)]) + "\t"+  t.strftime("%H:%M:%S  %m/%d")  
                        except:
                            pass

        if task==2 and arduino.sensorCalibrated==False:
            task==False;
            print "You need to calibrate the sensor first!"
        

main()

