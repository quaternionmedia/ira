import packet
from time import sleep
from datetime import datetime

LIGHTS_ON = 15

commands = [
    [75,0,0,0,80,8],
    [125,0,0,0,190, 10],
    [175,0,0,0,192]
    ]
delay = 10
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
