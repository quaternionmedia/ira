#!/usr/local/bin/python3.7

from starlette.applications import Starlette
from starlette.responses import HTMLResponse, PlainTextResponse, UJSONResponse
from starlette.endpoints import WebSocketEndpoint

import uvicorn
import packet

app = Starlette(template_directory='templates')
current = ''

channels = ['program', 'red', 'green', 'blue', 'speed', 'eyesize', 'arg1', 'arg2']

@app.route('/ira')
def ira(request):
    program = request.query_params['program'][:-1].split(',')
    program = [int(p) for p in program]
    print('ira got ', program)
    return PlainTextResponse(str(packet.send(*program)))

@app.route('/')
def home(request):
    return HTMLResponse(app.get_template('home.html').render(channels=channels))

if __name__ == '__main__':
    uvicorn.run(app, host='0.0.0.0', port=8000)
