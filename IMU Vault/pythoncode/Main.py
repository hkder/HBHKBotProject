# -*- coding: utf-8 -*-
"""
Created on Tue Oct 29 17:40:00 2019
​
@author: Hosung Kim
"""

# I2C device at 0x68
from pyfirmata import Arduino, util
import imaplib
import serial
import time


def getSerial():
    comNumber = "COM9"
    arduinoSerialData = serial.Serial(comNumber, 9600)
    arduinoSerialData.close()
    arduinoSerialData.open()

    while 1:
        # myData = str(arduinoSerialData.readline())
        # print(myData)
        msg = str(arduinoSerialData.readline())
        print(msg[2:len(msg)-5])


if __name__ == '__main__':
    getSerial()

'''
board = Arduino("COM9")
for x in range(5):
    board.digital[13].write(1)
    time.sleep(0.2)
    board.digital[13].write(0)
    time.sleep(0.2)
'''
'''
ser = serial.Serial("COM9", 9600)
ser.write()
ser.close()
ser.open()

line = str(ser.readline())
print(line)


try:
    while(1):
        line = str(ser.readline())
        items = line.split(",")
        print(line)
        #print("yaw:\t" + str(float(items[1])) + "\t\t pitch: \t" + str(float(items[2])) + "\t roll: \t" + str(float(items[3])))
        #time.sleep(0.5)
except:
    ser.close()
    exit()
'''
