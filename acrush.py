#!/usr/bin/python3.6

from sys import argv
from requests import get
#from simple_requests import Requests
import asyncio
import concurrent.futures

addr = ['192.168.10.102:8000', '192.168.10.107:8000']

def crush(*values):
    reqs = []
    for a in addr:
        query = 'http://'
        query += a
        query += '/ira?'
        for i in range(len(values)):
            query += f'program={values[i]}&'
        print('sending ', query[:-1])
        # print(get(query[:-1]))
        reqs += query
    # map(reqs)
    # for response in Requests().swarm(reqs):
        # print(response)
    return reqs
if __name__ == '__main__':
    with concurrent.futures.ThreadPoolExecutor(max_workers=20) as executor:

        loop = asyncio.get_event_loop()
        futures = [ loop.run_in_executor( executor, get, i) for i in crush(*argv[1:]) ]
