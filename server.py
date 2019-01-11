#!/usr/local/bin/python3.7

import hug
import packet

current = ''

@hug.get('/ira')
def ira(program: hug.types.multiple):
    program = [int(p) for p in program]
    print('ira got ', program)
    return packet.send(*program)
