from Ball import *
from BowlingBall import *
from TennisBall import *
from MarbleBall import *
from Block import *
from BookBlock import *
from DominoBlock import *
from Segment import *
from RotatingSegment import *
from Connector import * 
from Board import * 
from User import *

from pygame.locals import *
import pymunk.pygame_util

import pymunk
import pygame
from pymunk.vec2d import Vec2d

import json, socket, pickle
import threading

''' TODO:
        create instances of class library
        let clients connect and interact with them
        use JSON based protocol (this is our choice) for client communication
        implement a console based test application as the client connecting to this server
        respect concurrency of the shared instances!
        use mutexes to provide integrity 
'''

''' General Rules:

    1    Write a server side listening a TCP/IP port, starting a thread/process per connection (call it agent)
    2    If project topic has User class, client should be able to create a new user or be authenticated as an existing user (you do not implement password this phase, just trust client).
    3    For each method, implement a text or json based client/server communication
    4    Respect concurrency of shared data
    5    Notifications should be transmitted over the network to the client. You are free to register agent object as observer or socket used by a generic notifier. Please note that this mechanism will be replaced by websocket in the fourth phase.
    6    Do not forget to write a test client which demonstrates the functionalities.
    7    In first phase, some methods were excluded for convenience, all given methods and functionalities should be implemented in this phase.

'''
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

class Server():
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.serverSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        self.serverSocket.bind((self.host, self.port))

    def listen(self):
        while True:
            self.serverSocket.listen(1)
            client, address = self.serverSocket.accept()
            threading.Thread(target = self.listenToClient, args = (client, address)).start()

    def listenToClient(self, client, address):
        userNameSet = False
        try:
            message = pickle.loads(client.recv(1024))

            if userNameSet == False:
                userName = message

                if userName:
                    print(bcolors.OKBLUE + 'User name set as: ' + bcolors.ENDC + userName)
                    userNameSet = True
                    client.send('Your user name is set as: ' + userName)
                    #print(bcolors.OKGREEN + 'Sent: ' + bcolors.ENDC + data)

            while True:
                try:
                    message = pickle.loads(client.recv(1024))
                    print('Got a message: ' + message)
                except:
                    client.close()
                    return False

        except:
            client.close()
            return False

if __name__ == "__main__":
    while True:
        try:
            host = 'localhost'
            port = 8000
            break
        except ValueError:
            pass

    Server(host, port).listen()