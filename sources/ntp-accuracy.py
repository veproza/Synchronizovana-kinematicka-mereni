import serial
import struct
import RPi.GPIO as GPIO
import time
import ntplib

print "starting"
GPIO.setmode(GPIO.BCM)
GPIO.setup(19, GPIO.IN)

def get_time():
    status = False
    last = True
    pulses = 0
    while True:
        status = GPIO.input(19)
        if(status and not last):
            pulses += 1
            t = time.time()
            return (t % 1) * 1e6
        last = status


ntpClient = ntplib.NTPClient()
for i in range(0, 1000):
    response = ntpClient.request('tik.cesnet.cz', version=4)
    off1 = get_time()
    off2 = response.offset * 1e6
    print str(off1) + "\t" + str(off2)
