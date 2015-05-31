import serial
import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(21, GPIO.IN)
ser = serial.Serial("/dev/ttyAMA0", 9600)

try:
    status = False
    last = False
    pulseAt = False
    while True:
        status = GPIO.input(21)
        if(status and not last):
            pulseAt = time.time()
        last = status
        waiting = ser.inWaiting()
        if waiting:
            if pulseAt:
                print time.time() - pulseAt
            else:
                ser.read(waiting)
            pulseAt = False



except KeyboardInterrupt:
    GPIO.cleanup()
    ser.close()
