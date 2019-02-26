#!/usr/local/bin/python3.7

from fastapi import FastAPI, Query
from starlette.responses import HTMLResponse
from starlette.templating import Jinja2Templates

from typing import List, Tuple
import uvicorn

import packet

app = FastAPI(template_directory='templates')
templates = Jinja2Templates('templates')
current = ''

channels = ['program', 'red', 'green', 'blue', 'speed', 'eyesize', 'arg1', 'arg2']

@app.get('/ira')
def ira(program: List[int] = Query(None)):
    print('ira got ', program)
    return packet.send(program)

@app.get('/')
def home():
    return HTMLResponse(templates.get_template('home.html').render(channels=channels))

if __name__ == '__main__':
    uvicorn.run(app, host='0.0.0.0', port=8000)
