import time
import serial

ser = serial.Serial(
        port='/dev/ttyS0', #Replace ttyS0 with ttyAM0 for Pi1,Pi2,Pi0
        baudrate = 9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
)
ser.flush()

speed = 0

while 1:
	speed = input("what is your desired speed?")
	print "desired speed is", speed
	ser.write("%d\n"%(speed))
	#time.sleep(1)
	speed += 1
