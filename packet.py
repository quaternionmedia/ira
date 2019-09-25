from serial import Serial
from cobs import cobs
from struct import pack
from serialports import serial_ports
port = serial_ports()[0]
BAUD = 9600
from time import sleep

s = Serial(port, BAUD, )

def send(*values):
    # print(f'serial sending {values}')
    p = cobs.encode(pack('>{}B'.format(len(values)), *values)) + b'\x00'
    print(f'sending: {p}')
    if s.write(p) is len(p):
        # return values
        sleep(.3)
        if s.in_waiting:
            result = s.read(s.in_waiting)
            if result[-1] is 0:
                results = result.split(b'\x00')
                results = results[:-1]
                for r in results:
                    decoded = cobs.decode(r)
                    values = [d for d in decoded]
                    return values
            else: print('no packet found. {results}')
        else:
            print('sent!')
    else: print("didn't write all values")
