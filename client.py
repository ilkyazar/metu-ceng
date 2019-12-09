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
import socket

from colors import colors

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

class Client():
    def __init__(self, host, sending_port, receiving_port):
        self.host = host

        self.sendingPort = sending_port
        self.receivingPort = receiving_port

        self.userNameSet = False
        self.user = None
        

    def connectToServer(self):
        self.sendSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sendSocket.connect((self.host, self.sendingPort))
        print(colors.RED + 'Client connected to' + colors.ENDC)
        print(self.sendSocket)

        self.receiveSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def getNotification(self):
        self.receiveSocket.connect((self.host, self.receivingPort))
        print(colors.RED + 'Client connected to' + colors.ENDC)
        print(self.receiveSocket)

        while True:
            try:
                notification = pickle.loads(self.receiveSocket.recv(2048))

                print('Message from server: ' + notification)
            except:
                self.receiveSocket.close()    
            
    def setUserName(self):
        if (self.userNameSet == False):
            data = input('Set a user name: ')
            
            while (data == 'admin'):
                print(colors.RED + 'You cannot set your user name as admin.' + colors.ENDC)
                data = input('Set a user name: ')

            self.userNameSet = True 
            self.user = User(data)

        userNameBytes = pickle.dumps(data)

        self.sendSocket.send(userNameBytes)
        threading.Thread(target = client.getNotification, args = ()).start()
        '''
        while True:
            message = input('Message: ')

            data = pickle.dumps(message)
            self.sendSocket.send(data)

            #received = sendSocket.recv(1024)

            #print("response: ", received)
        '''

    def initializeGame(self):
        pymunk.pygame_util.positive_y_is_up = False
        pygame.init()
        pygame.display.set_caption(self.user.getName())
        #clock = pygame.time.Clock()
        font = pygame.font.Font(None, 24)

    def createBoard(self, inputFile):
        newBoard = Board()
        newBoard.start((1./60.0), 60, 1000)
        newBoard.load(inputFile)
        options = pymunk.pygame_util.DrawOptions(newBoard.screen)

        self.createContainers(newBoard)

        newBoard.screen.fill(pygame.color.THECOLORS["white"])

        self.updateSpace(newBoard, options)

    def createContainers(self, board):
        box_size = 200
        w = board.screen.get_width()
        h = board.screen.get_height()
        for i in range(4):
            sw = pymunk.Segment(board.space.static_body, 
                (0, i*box_size), (w, i* box_size), 1)
            sw.friction = 1
            sw.elasticity = 1
            sh = pymunk.Segment(board.space.static_body, 
                (i*box_size, 0), (i*box_size, h-box_size), 1)
            sh.friction = 1
            sh.elasticity = 1
            board.space.add(sw, sh)

    def updateSpace(self, board, options):
        board.updateSpace()
        board.space.debug_draw(options)
        pygame.display.flip()
        clock = pygame.time.Clock()
        clock.tick(60)        

if __name__ == "__main__":
    while True:
        try:
            host = 'localhost'
            sending_port = 8000
            receiving_port = 5000
            break
        except ValueError:
            pass

    client = Client(host, sending_port, receiving_port)
    client.connectToServer()
    client.setUserName()
    client.initializeGame()
    client.createBoard('./inputs/test4.json')

