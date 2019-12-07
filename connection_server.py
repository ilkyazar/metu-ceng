#from connection import server
from server import Server
import json

import threading

host = 'localhost'
port = 8000

server = Server(host, port)

client_count = 2
i = 0
while i < client_count:

    server.accept()

    with open('./inputs/test1.json') as json_file:
        board_data = json.load(json_file)
        
    server.send(board_data)

    server.send('what is your username? ')
    
    #server.serverSocket.listen(1)
    server.recv()
    
    i+=1
    
server.close()


# https://www.binarytides.com/code-chat-application-server-client-sockets-python/