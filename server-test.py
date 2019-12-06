from server import Server
import json

host = 'localhost'
port = 8000


server = Server(host, port)
server.accept()


with open('test1.json') as json_file:
    board_data = json.load(json_file)

server.send(board_data)

data = server.recv()
print('Server received:', data)

server.send({'data': data})

server.close()