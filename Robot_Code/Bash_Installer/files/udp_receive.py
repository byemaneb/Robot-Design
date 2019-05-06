import socket
import struct

#C:\Users\berke\Documents\GitHub\Robot-Design\Robot_Code\UDPTestPi

UDP_IP = "172.20.10.8"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
	data,addr = sock.recvfrom(1024) # buffer size is 1024 bytes
	speed,turn,angle,xPosition,yPosition,globalAngle = struct.unpack("ffffff",data)
	#print("speed",data)
	print("speed",speed)
	print("turn",turn)
	print("angle",angle)
	print("xPosition",xPosition)
	print("yPosition",yPosition)
	print("globalAngle",globalAngle)
	
	