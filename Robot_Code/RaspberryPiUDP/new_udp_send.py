import curses
import socket
import struct
import sys

UDP_IP_SEND = '192.168.1.55'
UDP_PORT_SEND = 5005

UDP_IP_RECEIVE = "192.168.1.100"
UDP_PORT_RECEIVE = 4242

send = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

recieve = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
recieve.bind((UDP_IP_RECEIVE,UDP_PORT_RECEIVE))




speed = 0
angle = 0
returnData = False

# get the curses screen window
screen = curses.initscr()

# turn off input echoing
curses.noecho()

# respond to keys immediately (don't wait for enter)
curses.cbreak()

# map arrow keys to special values
screen.keypad(True)

try:
	while True:

		key = screen.getch()
		screen.clear()
		if key == ord('q'):
			screen.addstr(0, 0, "You pressed the q key.")
			sys.exit(" You have sucessfully exited the program")
			break
		elif key == curses.KEY_RIGHT:
			screen.addstr(0, 0, 'right')
			angle += 15
		elif key == curses.KEY_LEFT:
			screen.addstr(0, 0, 'left ')
			angle -= 15
		elif key == curses.KEY_UP:
			screen.addstr(0, 0, 'up   ')
			speed +=10
			if speed <70:
				speed = 70
			if speed > 255:
				speed = 255
		elif key == curses.KEY_DOWN:
			screen.addstr(0, 0, 'down ')
			speed -=10
			if speed <70:
				speed = 0
		elif key == ord('w'):
			screen.addstr(0, 0, "You pressed the w key.")
			angle = 270
		elif key == ord('s'):
			screen.addstr(0, 0, "You pressed the a key.")
			angle = 90
		elif key == ord('d'):
			screen.addstr(0, 0, "You pressed the s key.")
			angle = 360
		elif key == ord('a'):
			screen.addstr(0, 0, "You pressed the d key.")
			angle = 180
		elif key == ord(' '):
			screen.addstr(0, 0, "You pressed the space key.")
			speed = 0
		elif key == ord('i'):
			screen.addstr(0, 0, "You pressed the i key.")
			returnData = True
		else:
			screen.addstr(0, 0, "This program doesn't know that key...")
		if angle > 360:
			angle = angle - 360
		elif angle < 0:
			angle = angle + 360
		screen.addstr(1, 0, str(speed))
		screen.addstr(2, 0, str(angle))
		screen.addstr(3, 0, str(returnData))

		#pack and send out UDP packet
		sendOut = struct.pack("@ff",speed,angle)
		send.sendto(sendOut, (UDP_IP_SEND, UDP_PORT_SEND))

		if returnData == True:
			data,addr = recieve.recvfrom(1024) # buffer size is 1024 bytes 
			heading,xPos,yPos,zPos,ax,ay,az,mx,my,mz = struct.unpack("dddddddddd",data)
			screen.clear()
			screen.addstr(0, 0, "heading")
			screen.addstr(1, 0, str(heading))
			screen.addstr(2, 0, "xPos")
			screen.addstr(3, 0, str(xPos))
                        screen.addstr(4, 0, "yPos")
                        screen.addstr(5, 0, str(yPos))
                        screen.addstr(6, 0, "zPos")
                        screen.addstr(7, 0, str(zPos))
                        screen.addstr(8, 0, "ax")
                        screen.addstr(9, 0, str(ax))
                        screen.addstr(10, 0, "ay")
                        screen.addstr(11, 0, str(ay))
                        screen.addstr(12, 0, "az")
                        screen.addstr(13, 0, str(az))
                        screen.addstr(14, 0, "mx")
                        screen.addstr(15, 0, str(mx))
                        screen.addstr(16, 0, "my")
                        screen.addstr(17, 0, str(my))
                        screen.addstr(18, 0, "mz")
                        screen.addstr(19, 0, str(mz))

			returnData = False





finally:
	# shut down cleanly
	curses.nocbreak();screen.keypad(0);curses.echo()
	curses.endwin()
