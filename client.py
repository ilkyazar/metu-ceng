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
        self.boarNameSet = False
        self.user = None
        

    def connectToServer(self):
        try:
            self.sendSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sendSocket.connect((self.host, self.sendingPort))
            print(colors.RED + 'Client connected to' + colors.ENDC)
            print(self.sendSocket)
        except:
            self.sendSocket.close()


        try:
            self.receiveSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.receiveSocket.connect((self.host, self.receivingPort))
            print(colors.RED + 'Client connected to' + colors.ENDC)
            print(self.receiveSocket)
        except:
            self.receiveSocket.close()

    def setUserName(self):
        if (self.userNameSet == False):
            data = input('Set a user name: ')
            
            while (data == 'admin'):
                print(colors.RED + 'You cannot set your user name as admin.' + colors.ENDC)
                data = input('Set a user name: ')
            self.userName = data
            data = 'u/' + str(data)
                
            userNameBytes = pickle.dumps(data)
            self.sendSocket.send(userNameBytes)
            self.receiveSocket.send(userNameBytes)
        
    def setBoardName(self):
        if (self.userNameSet == True and self.boarNameSet == False):
            data = input('Select a board: ')
            
            self.boarNameSet = True 
            self.boardName = data

            boardNameBytes = pickle.dumps(data)
            self.sendSocket.send(boardNameBytes)

    def sendAction(self):
        #while True:
        data = input('New Action: ')
        actionBytes = pickle.dumps(data)
        self.sendSocket.send(actionBytes)
        
    def getNotification(self):

        while True:
            try:
                print(colors.writeRed('GONnA GET NOTIFICATIIOOON'))
                data = self.receiveSocket.recv(2048)
                notification = pickle.loads(data)

                print(colors.writeYellow('Notification from server: ') + notification)

                if (notification == 'Set another user name for gods sake!'):
                    self.setUserName()
                
                elif(notification == 'You chose wisely.'):
                    self.userNameSet = True 
                    self.setBoardName()
                    
                else:
                    self.sendAction()

            except:
                print(colors.writeRed('Receive socket is closed.'))
                self.receiveSocket.close()    
                break



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
    threading.Thread(target = client.getNotification, args = ()).start()
    client.setUserName()
    
    
    #client.createBoard('./inputs/test4.json')

