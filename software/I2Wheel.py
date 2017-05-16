import RPi.GPIO as GPIO
import smbus, time, sys

Address = int("0x"+str(sys.argv[1]), 16)
Bank = sys.argv[2].upper()
Bit = sys.argv[3]
Motor = int(sys.argv[4])
Rotate = int(sys.argv[5])-1

if Bank == "A":
	Bank = 0x12
elif Bank == "B":
	Bank = 0x13
else:
	Bank = 0x00

if Bit == "1":
	Byte = 0x01
elif Bit == "2":
	Byte = 0x02
elif Bit == "3":
	Byte = 0x04
elif Bit == "4":
	Byte = 0x08
elif Bit == "5":
	Byte = 0x10
elif Bit == "6":
	Byte = 0x20
elif Bit == "7":
	Byte = 0x40
elif Bit == "8":
	Byte = 0x80

Bus = smbus.SMBus(1)
GPIO.setmode(11)
GPIO.setup(Motor, GPIO.OUT)

def Write(addr, reg, data):
	Bus.write_byte_data(addr, reg, data)
	return

def Read(addr, reg):
	return Bus.read_byte_data(addr, reg)

def Detect():
	global Count
	global Running
	global Rotate
	Count = Count + 1
	sys.stdout.write("{} ".format(Count))
	sys.stdout.flush()
	if Count == 20:
		Rotate = Rotate - 1
		Count = 0
		sys.stdout.write("\n")
		sys.stdout.flush()
	if Count == 16 and Rotate == 0:
		sys.stdout.write("17 18 19 20\n")	# Fake writing out the next 4, Wheel will still spin after shutdown, which seems to be 4 / 20.
		print "Finished?"
		GPIO.output(Motor, 0)
		Running = False
Count = 0
Last = 0
Write(Address, 0x00, 0xFF)
Write(Address, 0x01, 0xFF)
Write(Address, Bank, 0x00)

Running = True
GPIO.output(Motor, 1)
while Running:
	Write(Address, Bank, 0xff)
	Data = Read(Address, Bank)
	Ret = Data & Byte
	if Ret == Byte:
		Cur = 1
	else:
		Cur = 0
	if Cur == 0 and Last == 1:
		Detect()
	Last = Cur

GPIO.output(Motor, 0)
GPIO.cleanup()
