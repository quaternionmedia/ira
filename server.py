#!/usr/local/bin/python3.7

import hug
import packet
from jinja2 import FileSystemLoader, Environment

templates = Environment(loader=FileSystemLoader("templates"))

current = ''

@hug.get('/ira')
def ira(program: hug.types.multiple):
    program = [int(p) for p in program]
    print('ira got ', program)
    return packet.send(*program)

@hug.get('/', output=hug.output_format.html)
def home():
    return templates.get_template('home.html').render()
