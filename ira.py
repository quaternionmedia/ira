#!/usr/local/bin/python3.7

from fastapi import FastAPI, Query
from starlette.responses import HTMLResponse
from starlette.staticfiles import StaticFiles
from starlette.templating import Jinja2Templates
from async_exec import run
from typing import List, Tuple
import uvicorn

import packet

app = FastAPI(template_directory='templates')
app.mount('/static', StaticFiles(directory='static'), name='static')

templates = Jinja2Templates('templates')
current = ''

channels = ['program', 'red', 'green', 'blue', 'speed', 'eyesize', 'arg1', 'arg2']

@app.get('/ira')
async def ira(program: List[int] = Query(None)):
    print('ira got ', program)
    return packet.send(program)

@app.get('/upload')
async def upload():
    arduinoType = 'nano:cpu=atmega328old'
    compiled = f'arduino-cli compile -b arduino:avr:{arduinoType} generator'
    results = await run(compiled.split(' '))
    uploaded = f'arduino-cli upload -b arduino:avr:{arduinoType} -p /dev/tty.usbserial-1430 -v generator'
    results += await run(uploaded.split(' '))
    return results

@app.get('/')
async def home():
    return HTMLResponse(templates.get_template('home.html').render(channels=channels))

@app.get('/paper')
async def paper():
    return HTMLResponse(templates.get_template('paper.html').render())


if __name__ == '__main__':
    uvicorn.run(app, host='0.0.0.0', port=8000)
