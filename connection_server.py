#from connection import server
from server import Server
import json

import threading

host = 'localhost'
port = 8000

server = Server(host, port)
server.accept()
while True:

    
    #with open('./inputs/test1.json') as json_file:
    #    board_data = json.load(json_file)

    board_data = input()
        
    server.send(board_data)
    server.serverSocket.listen(1)
    server.recv()
    


# https://www.binarytides.com/code-chat-application-server-client-sockets-python/