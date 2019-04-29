import socket
import struct

		

UDP_IP = "172.20.10.8"
UDP_PORT = 5005


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))


while True:
	data,addr = sock.recvfrom(1024) # buffer size is 1024 bytes

	speed,angle = struct.unpack("@fff",data)
	#print("speed",struct.unpack("@fff",data))
	print("speed",speed)
	print("angle",angle)
	
	