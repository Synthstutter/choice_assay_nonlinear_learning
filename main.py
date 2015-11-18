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
import random
import threading


    
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
       
    def readline(self):
        return self.ser.readline()

class matrix_thread(threading.Thread):
    def run(self):
        global matrix_pattern
        global arduino
        patterns=["3","4","5","6"]
        matrix_pattern=random.choice(patterns)
        arduino.write(matrix_pattern)
        print "matrix pattern: " + matrix_pattern + "\r"
        t.sleep(90)
        matrix_pattern="7"
        arduino.write(matrix_pattern)
        print "matrix off\r"
        t.sleep(30)

def current_time():
    return t.strftime("Time: %H:%M:%S")


def main():
    running = True

    arduino.write("1")      # calibrate the arduino
    arduino.write("2")      # start recording
    print "\n..................running..................."
    print current_time()
    matrix_thread().start()
    while running:
        if not os.path.exists("choice_assay_data/"):
            os.makedirs("choice_assay_data/")

        fileTimeName1=(t.strftime('%Y_%m_%d.txt'))
        filename1= "choice_assay_data/" + fileTimeName1
        count=[0,0]
        f=open(filename1,"a")

        # if arduino.ser.inWaiting()>0:
        #     item=arduino.readline()
        #     try:
        #         count[int(item)] += 1
        #         f.write("sensor ")
        #         f.write(str(item))
        #         f.write("\tdate_time: ")
        #         f.write(str(t.localtime()))
        #         f.write("\tcount:")
        #         f.write(str(count[int(item)]))
        #         f.write("\n")
        #         print str(item) + "\tCount: "+ str(count[int(item)]) + "\t"+  t.strftime("%H:%M:%S  %m/%d")  
        #     except:
        #         pass

arduino=Arduino()
matrix_pattern = False
main()

