import serial
import struct
import RPi.GPIO as GPIO
import os
from datetime import datetime, timedelta

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
            offset = datetime.now().microsecond
            if offset > 0.5 * 1e6:
                offset -= 1e6
            return offset
        last = status

print "Current offset:"
for i in range(0, 2):
    offset1 = get_time()
    print(offset1)

corrected = datetime.now() - timedelta(microseconds=offset1)
str = corrected.strftime('%H:%M:%S.%f')
os.system('date -s %s' % str)

print("Correcting with offset");
print(offset1)

print "After GPS correction: "
for i in range(0, 2):
    offset2 = get_time()
    print(offset2)

offset3 = offset2

while abs(offset3) > 100:
    corrected = datetime.now() - timedelta(microseconds=2 * offset2)
    str = corrected.strftime('%H:%M:%S.%f')
    os.system('date -s %s' % str)
    print("Correcting with offset");
    print(offset2)

    print "After call-time correction:"
    offset3 = get_time()
    print(offset3)
    if offset3 < 20000:
        offset2 += offset3
    else:
        offset2 = offset3 / 2
