#!/usr/bin/python3.7

# multi ira client
# fires multiple ira command over synchronous http

from sys import argv
from requests import get
addr = ['192.168.10.102:8000', '192.168.10.107:8000']
def iras(*values):
    for a in addr:
        query = 'http://'
        query += a
        query += '/ira?'
        for i in range(len(values)):
            query += f'program={values[i]}&'
        print('sending ', query[:-1])
        print(get(query[:-1]))

if __name__ == '__main__':
    iras(*argv[1:])
