from serial import Serial
from cobs import cobs
from struct import pack
from serialports import serial_ports
port = serial_ports()[0]
BAUD = 1200
from time import sleep

s = Serial(port, BAUD, )

def send(*args):
	p = cobs.encode(pack('>{}B'.format(len(args)), *args)) + b'\x00'
	s.write(p)
	sleep(.2)
	if s.in_waiting:
		result = s.read(s.in_waiting)
		if result[-1] == 0:
			decoded =cobs.decode(result[:-1])
			values = []
			for d in decoded:
				values.append(d)
			print(values)
		else:
			print(result)
