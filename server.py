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

class Server():
    def __init__(self, host, listening_port, notifying_port):
        self.host = host

        self.listeningPort = listening_port
        self.notifyingPort = notifying_port

        self.listenSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.listenSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.listenSocket.bind((self.host, self.listeningPort))

        self.notifySocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.notifySocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.notifySocket.bind((self.host, self.notifyingPort))       

    def listen(self):
        while True:
            self.listenSocket.listen(1)
            client, address = self.listenSocket.accept()

            print(colors.GREEN + 'New client connected' + colors.ENDC)
            print(address)

            threading.Thread(target = self.listenToClient, args = (client, address)).start()


    def listenToClient(self, client, address):
        userNameSet = False
        try:
            message = pickle.loads(client.recv(1024))

            if userNameSet == False:
                userName = message

                if userName:
                    print(colors.BLUE + 'User name set as: ' + colors.ENDC + userName)
                    userNameSet = True
                    threading.Thread(target = self.notifyClient, args = ()).start()
                    #notfiyClient()
            '''
            while True:
                try:
                    message = pickle.loads(client.recv(1024))
                    print(colors.GREEN + userName + ': ' + colors.ENDC + message)
                    #print('Got a message: ' + message + ' from ' + userName)
                except:
                    client.close()
                    return False
            '''

        except:
            client.close()
            return False

    def notifyClient(self):
        while True:
            self.notifySocket.listen(1)
            client, address = self.notifySocket.accept()

            print(colors.GREEN + 'New client connected' + colors.ENDC)
            print(address)
            print(self.notifySocket)

            self.sendNotification(client, address)

            self.initializeGame()
            self.createBoard()

            #self.sendOtherNotification(client, address)

    def sendNotification(self, client, address):
        print(colors.YELLOW + 'Sending notification' + colors.ENDC)
        print(colors.GREEN + str(client) + colors.GREEN)
        #print(colors.BLUE + userName + colors.BLUE)
        data = 'Hello this is server.'
        notification = pickle.dumps(data)
        client.send(notification)
        print(colors.YELLOW + 'Notificiation sent' + colors.ENDC)
    
    def sendOtherNotification(self, client, address):
        print(colors.YELLOW + 'Sending notification' + colors.ENDC)
        data = 'Hello this is server, AGAIIIINNN.'
        notification = pickle.dumps(data)
        client.send(notification)
        print(colors.YELLOW + 'Notificiation sent' + colors.ENDC)

    def initializeGame(self):
        pymunk.pygame_util.positive_y_is_up = False
        pygame.init()
        #clock = pygame.time.Clock()
        font = pygame.font.Font(None, 24)

    def createBoard(self):
        newBoard = Board()
        newBoard.start((1./60.0), 60, 1000)
        newBoard.load('./inputs/test4.json')
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
            listening_port = 8000
            notifying_port = 5000
            break
        except ValueError:
            pass

    Server(host, listening_port, notifying_port).listen()