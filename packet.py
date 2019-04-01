from serial import Serial
from cobs import cobs
from struct import pack
from serialports import serial_ports
port = serial_ports()[-1]
BAUD = 1200
from time import sleep

s = Serial(port, BAUD, )

def send(values):
	print(f'serial sending {values}')
	p = cobs.encode(pack('>{}B'.format(len(values)), *values)) + b'\x00'
	if s.write(p) is len(p):
	    return values
	# sleep(.3)
	# if s.in_waiting:
	# 	result = s.read(s.in_waiting)
	# 	if result[-1] is 0:
	# 		results = result.split(b'\x00')
	# 		results = results[:-1]
	# 		for r in results:
	# 			decoded = cobs.decode(r)
	# 			values = [d for d in decoded]
	# 			return values
	# 	else:
	# 		print(result)
