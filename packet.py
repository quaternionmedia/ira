from serial import Serial
from cobs import cobs
from struct import pack
from serialports import serial_ports
port = serial_ports()[0]
BAUD = 9600
from time import sleep

s = Serial(port, BAUD, )

def send(*args):
	p = cobs.encode(pack('>{}B'.format(len(args)), *args)) + b'\x00'
	s.write(p)
	sleep(.1)
	if s.in_waiting:
		print(s.read(s.in_waiting))
