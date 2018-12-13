import packet
from time import sleep
from datetime import datetime
commands = [
    [125,50,0,0,190],
    [204,0,255,0,190],
    [127,0,0,0,180]
    ]
delay = 15
print('loading...')
sleep(2)
while datetime.now().hour > 15 :
    for cmd in commands:
        print('sending {}'.format(cmd))
        packet.send(*cmd)
        sleep(delay)
