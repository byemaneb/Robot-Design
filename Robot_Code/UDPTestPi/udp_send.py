import socket
import struct

class user:
	def __init__(self,speed= 0,angle = 0):
		self.speed = speed
		self.angle = angle
		
	def getSpeed(self):
		return self.speed  
	
	def getAngle(self):
		return self.angle


UDP_IP = "172.20.10.7"
UDP_PORT = 5005
MESSAGE = "Hello, World!"


print ("UDP target IP:", UDP_IP)
print ("UDP target port:", UDP_PORT)
#print ("message:", MESSAGE)

while True:
	#recieve user data
	print("enter desired speed:")
	speed = input()
	speed = float(speed)
	print ("enter desired angle")
	angle = input()
	angle = float(angle)
	sendOut = struct.pack("@ff",speed,angle)
	#value = input()
	#value = int(value)
	#sendOut = struct.pack("i",value)
	sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) # UDP
	sock.sendto(sendOut, (UDP_IP, UDP_PORT))




