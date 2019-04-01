from dash import Dash
from dash.dependencies import Input, Output, State
import dash_core_components as dcc
import dash_html_components as html
import dash_daq as daq
import packet

sliderNames = ['speed', 'eyesize', 'arg1', 'arg2']

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
                    value={'hex': '#000000'}),
    html.Div(style={'display': 'flex', 'padding-top': '60px'},
            children=[
        html.P('Program', className='one columns'),
        daq.Slider(id='program', min=0, max=255, step=25, value=0, size=700,
                    className='eleven columns',
                    handleLabel={'showCurrentValue': True, 'label': ' '},
                    marks={i:{'label': str(i)} for i in range(0, 255, 25)})]),
    *[html.Div(style={'display': 'flex', 'padding-top': '60px'},
            children=[
        html.P(i, className='one columns'),
        daq.Slider(id=i, min=0, max=255, step=1, value=0, size=700,
                    className='eleven columns',
                    handleLabel={'showCurrentValue': True, 'label': ' '})]) for i in sliderNames],
    html.Div(style={'display': 'flex', 'padding-top': '60px'}, children=[
        html.H3('Value'),
        html.Div(id='iraValues'),]),
], style=style, className='container', )


@app.callback(Output('iraValues', 'children'),
            [Input('program', 'value'),
            Input('color-picker', 'value'),
            *[Input(i, 'value') for i in sliderNames]],
            )
def updateRange(*v):
    c = v[1]['hex']
    p = [v[0], int(c[1:2], 16), int(c[3:4], 16), int(c[5:6], 16), *v[2:]]
    print('updating', v, p)
    return packet.send(p)



if __name__ == '__main__':
    app.run_server(host='0.0.0.0', port=8000, debug=True)
