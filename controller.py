import packet
from time import sleep
from datetime import datetime

LIGHTS_ON = 15

commands = [
    [125,50,0,0,190],
    [204,0,255,0,190],
    [127,0,0,0,180]
    ]
delay = 15
print('loading...')
sleep(2)
while True:
    if datetime.now().hour >= LIGHTS_ON:
        for cmd in commands:
            print('sending {}'.format(cmd))
            packet.send(*cmd)
            sleep(delay)
    else:
        packet.send(0,0,0,0)
        t = abs(LIGHTS_ON - datetime.now().hour)
        print('goodnight! See you in {} hours!'.format(t))
        sleep(t*3600)
