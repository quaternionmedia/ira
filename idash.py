from dash import Dash
from dash.dependencies import Input, Output, State
import dash_core_components as dcc
import dash_html_components as html
import dash_table
from dash.exceptions import PreventUpdate
import dash_daq as daq
import packet
from time import time
from subprocess import run, PIPE

sliderNames = ['speed', 'hue speed', 'eyesize', 'arg']
parameterNames = ['program', 'r', 'g', 'b', *sliderNames]
program = []
fxNames = ['wash', 'cylon', 'marquee', 'wipe', 'waves', 'rainbow', 'glitter', 'chaser', 'hueCycle', 'progress']
lastUpdate = time()

app = Dash()
app.title = 'Ira'
style = {'width':'90%',
    'padding': '10px 10px 10px 10px',
    'marginLeft': 'auto',
    'marginRight': 'auto',
    # 'width': '850',
    # 'height': '685',
    'boxShadow': '0px 0px 5px 5px rgba(204,204,204,0.4)',}

app.layout = html.Div([
    html.H1('Ira controller'),
    daq.ColorPicker(id='color-picker',
                    label='select a color',
                    value={'rgb': {'r': 0, 'g': 0, 'b': 0}}),
    html.Div(style={'display': 'flex', 'padding-top': '60px'},
            children=[
        html.P('Program', className='one columns'),
        daq.Slider(id='program', min=0, max=255, step=26, value=0, size=700,
                    updatemode='drag',
                    className='eleven columns',
                    handleLabel={'showCurrentValue': True, 'label': ' '},
                    marks={i*26:{'label': fxNames[i]} for i in range(len(fxNames))})]),
    *[html.Div(style={'display': 'flex', 'padding-top': '60px'},
            children=[
        html.P(i, className='one columns'),
        daq.Slider(id=i, min=0, max=255, step=1, value=0, size=700,
                    updatemode='drag',
                    className='eleven columns',
                    handleLabel={'showCurrentValue': True, 'label': ' '},
                    marks={ n: {'label': n} for n in range(0, 255, 32)})]) for i in sliderNames],
    html.Div(style={'display': 'flex', 'padding-top': '60px'}, children=[
        html.H3('Value'),
        html.Div(id='iraValues'),]),
    dash_table.DataTable(id='datatable',
                        columns=[{'name': i, 'id': i} for i in parameterNames],
                        ),
    html.Br(),
    html.Div(id='uploader', children=[
        html.Button('upload', id='upload', type='button'),
        html.Pre(id='uploadResults'),]),

], style=style, className='container', )


@app.callback(Output('iraValues', 'children'),
            [Input('program', 'value'),
            Input('color-picker', 'value'),
            *[Input(i, 'value') for i in sliderNames]],
            )
def updateRange(*v):
    global lastUpdate
    if time() - lastUpdate < .05:
        raise PreventUpdate
        return
    c = v[1]['rgb']
    p = [v[0], c['r'], c['g'], c['b'], *v[2:]]
    print('updating', v, p)
    lastUpdate = time()
    return packet.send(p)

@app.callback(Output('uploadResults', 'children'), [Input('upload', 'n_clicks')])
def upload(n):
    if n:
        arduinoType = 'nano:cpu=atmega328old'
        compiled = f'arduino-cli compile -b arduino:avr:{arduinoType} generator'
        results = run(compiled.split(' '), capture_output=True).stdout
        uploaded = f'arduino-cli upload -b arduino:avr:{arduinoType} -p /dev/tty.usbserial-1430 -v generator'
        results += run(uploaded.split(' '), capture_output=True).stdout
        return results.decode()

if __name__ == '__main__':
    app.run_server(host='0.0.0.0', port=8000, debug=True)
