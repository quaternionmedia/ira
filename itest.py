from fastapi import FastAPI
from idash import createApp #app as idash
from uvicorn import run
from starlette.middleware.wsgi import WSGIMiddleware
from starlette.routing import Mount, Route, Router

app = FastAPI() #openapi_prefix='/f')
d = createApp(requests_pathname_prefix='/app/')
app.mount('/app', WSGIMiddleware(app=d.server))
# app = Router([
#     Mount('/f', f ),
#     # Route('/', endpoint=f, methods=['GET']),
#     Mount('/app', WSGIMiddleware(app=d.server))
# ])

# f.mount('/app', WSGIMiddleware(idash.server))
if __name__ == '__main__':
    run(app, host='0.0.0.0', port=8888, debug=True)
