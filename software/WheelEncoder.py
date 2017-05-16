import time, sys
import RPi.GPIO as GPIO

Encode = int(sys.argv[1])
Rotate = int(sys.argv[2]) - 1
M1 = int(sys.argv[3])

Pins = [17, 18, 27, 22, 23, 24, 25, 4, 28, 29, 30, 31]
GPIO.setmode(11)
for A in Pins:
	GPIO.setup(A, GPIO.OUT)
	GPIO.output(A, 0)

GPIO.setup(Encode, GPIO.IN, GPIO.PUD_UP)
GPIO.setup(M1, GPIO.OUT)
CurRot = 0
Rotations = 0
Rev = 0
Last = 0
In = 0
Running = 1

def Wheel():
	global Rotate
	global CurRot
	global Running
	global Rotations
	CurRot = CurRot + 1
	sys.stdout.write(str(CurRot)+" ")
	sys.stdout.flush()
	if CurRot == 20 and Rotations == Rotate:
		GPIO.output(M1, 0)
		print "\n{} Rotations!".format(Rotate+1)
		Running = 0
	elif CurRot == 20:
		Rotations = Rotations + 1
		sys.stdout.write("!\n")
		sys.stdout.flush()
		CurRot = 0
GPIO.output(M1, 1)
while Running:
	In = GPIO.input(Encode)
	if GPIO.input(Encode) == 0 and Last == 1:
		Wheel()
	Last = In

GPIO.cleanup()
