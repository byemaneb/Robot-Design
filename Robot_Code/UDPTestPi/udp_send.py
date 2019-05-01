import socket
import struct

UDP_IP = "172.20.10.7"
UDP_PORT = 5005
MESSAGE = "Hello, World!"


print ("UDP target IP:", UDP_IP)
print ("UDP target port:", UDP_PORT)


speed = 0
turn = 0
angle = 0
returnData = False

while True:
	#recieve user data
	print("Rotate Robot: i: 0 degrees (North), l: 90 degrees (East), k: 180 degrees (South), and j: 270 degrees (West) ")
	print("Move Robot: w: Forward(incresase speed), d: Turn Right, s: Reverse(decrease speed), a: Turn Left ")
	print("Stop Robot: space ")
	key = input()
	
	if key == "w":
		speed += 10
		
		if speed < 50:
			speed = 50 
		
		if speed > 255:
			speed = 255 

	elif key == "s":
		speed -= 10
		
		if speed < 50:
			speed = 0 

	elif key == "d":
		turn = 1

	elif key == "a":
		turn = -1
	elif key == "i":
		angle = 270
	elif key == "k":
		angle = 90
	elif key == "l":
		angle = 360
	elif key == "j":
		angle = 180
	elif key == " ":
		speed = 0
	elif key == "q":
		returnData = True	
		
	print("speed", speed)
	print("turn ", turn)
	print("angle ", angle)
	print("returnData",returnData)


	#pack and send out UDP packet
	sendOut = struct.pack("@fff?",speed,turn,angle,returnData)
	sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	sock.sendto(sendOut, (UDP_IP, UDP_PORT))
	
	turn = 0
	angle = 0
	returnData = False




