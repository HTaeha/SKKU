from socket import *
import time
import os
from threading import Thread

serverIP = ''
serverPort = 10080
ADDR = (serverIP,serverPort)
chunk_size = 1024 * 4096

serverSocket = socket(AF_INET, SOCK_STREAM)

serverSocket.bind(ADDR)
serverSocket.listen(10)
print('The TCP server is ready to receive.')

def connect():
    connectionSocket, addr = serverSocket.accept()

    t = Thread(target = connect)
    t.start()

    login_flag = 0
    cookie_flag = 0
    msg = connectionSocket.recv(chunk_size).decode()
    #print(msg)
    file_split = msg.split(' ')
    file_name = file_split[1]
    
    file_content = msg.split('\r\n')
    for i, data in enumerate(file_content):
        if data.count('Cookie:')>0:
            cookie_str = data.split(' ')
            ID = cookie_str[1].split('=')[1]
            ID = ID[:-1]
            passwd = cookie_str[2].split('=')[1]
            passwd = passwd[:-1]
            cookie_flag = 1
            break
    
    if file_name == '/' and file_split[0] == 'GET':
        with open('index.html', 'rb') as f:
            file_size = os.path.getsize('index.html')
            connectionSocket.send('HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:{}\r\n\r\n'.format(file_size).encode())
            while True:
                buffer = f.read(chunk_size)
                if not buffer: break
                connectionSocket.send(buffer)
    
    elif cookie_flag == 1 or file_split[0] == 'POST':
        if file_split[0] == 'POST':
            for i, data in enumerate(file_content):
                if data.count('Content-Disposition:')>0:
                    if data[-3]=='I' and data[-2]=='D':
                        id_str = file_content[i+2]
                    else:
                        passwd_str = file_content[i+2]
            with open('secret.html', 'rb') as f:
                file_size = os.path.getsize('secret.html')
                connectionSocket.send('HTTP/1.1 200 OK\r\nSet-Cookie: id_cookie={}; max-age=30\r\nSet-Cookie: passwd_cookie={}; max-age=30\r\nContent-Type: text/html\r\nContent-Length:{}\r\n\r\n'.format(id_str,passwd_str, file_size).encode())
                while True:
                    buffer = f.read(chunk_size)
                    if not buffer: break
                    connectionSocket.send(buffer)

        elif file_split[0] == 'GET':
            file_name = file_name.split('/')[1]
            file_format = file_name.split('.')[-1]

            if file_name == 'cookie.html':
                if cookie_flag == 1:
                    connectionSocket.send(b'HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n')
                    connectionSocket.send(('<html><body><h1>Hello '+ID+'</h1><br><br><h1>seconds left until your cookie expires.</h1></body></html>').encode())
                else:
                    connectionSocket.send(b'HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\n\r\n')
                    connectionSocket.send(b'<html><body><h1>403 Forbidden</h1></body></html>')
            elif not os.path.isfile(file_name):
                connectionSocket.send(b'HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n')
                connectionSocket.send(b'<html><body><h1>404 Not Found</h1></body></html>')
            else:
                with open(file_name,'rb') as f:
                    file_size = os.path.getsize(file_name)

                    
                    connectionSocket.send('HTTP/1.1 200 OK\r\nContent-Length:{}\r\n\r\n'.format(file_size).encode())
                    while True:
                            buffer = f.read(chunk_size)
                            if not buffer: break
                            connectionSocket.send(buffer)
                
    else:
        connectionSocket.send(b'HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\n\r\n')
        connectionSocket.send(b'<html><body><h1>403 Forbidden</h1></body></html>')
             
    connectionSocket.close()

connect()
