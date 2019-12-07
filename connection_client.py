#from connection import client
import threading
from client import Client

host = 'localhost'
port = 8000

newthread = Client(host, port)
newthread.start()

#hello_message = client.recv()

#print("hello msg from server", hello_message)

#client.send("{'some_list': [123, 456]}")
#response = client.recv()
#print('Response is: ', response)


#client.close()
