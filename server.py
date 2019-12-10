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

        #boardDict holds boardname-board pairs
        self.boardDict = {}
        #username - client pairs
        self.userDict = {}   

    def startListening(self):
        
        threading.Thread(target = self.listenRequest, args = ()).start()
        threading.Thread(target = self.listenNotify, args = ()).start()

    def listenRequest(self):
        print("Listen request thread is started.")
        try:
            self.requestSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.requestSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.requestSocket.bind((self.host, self.listeningPort))
            self.requestSocket.listen()

        except:
            print('Exception') 

        while True:
            
            client, address = self.requestSocket.accept()

            print(colors.GREEN + 'New client connected' + colors.ENDC + " " + str(address))

            threading.Thread(target = self.listenToClient, args = (client, address)).start()

    def listenToClient(self, client, address):
        print("Listening to client: " + str(address))
        userNameSet = False
        userName = ''
        boardNameSet = False
        boardName = ''

        while True:
            try:
                data = client.recv(1024)
                message = pickle.loads(data)
                if userNameSet == False:
                    userName = message
                    newUser = User(userName)
                    userNameSet = True 

                elif boardNameSet == False:
                    boardName = message
                    print(colors.BLUE + 'Board name set as: ' + colors.ENDC + boardName)
                    boardNameSet = True

                    if boardName in self.boardDict.keys():
                        print('This board name is already in the board dictionary. ')
                        print(self.userDict)
                        self.attachUser(newUser, self.boardDict[boardName])
                        print(self.boardDict[boardName].users.keys())
                    else:
                        newBoard = self.createBoard(boardName)
                        self.boardDict[boardName] = newBoard
                        print(self.userDict)
                        self.attachUser(newUser, newBoard)
                        print(self.boardDict[newBoard].users.keys())

                    print('Board dictionary is: ')
                    print(self.boardDict)

                else:
                    print(color.writeRed('Other Message Received'))

            except:
                client.close()
                return False


    def listenNotify(self):
        print("Listen notify thread is started.")
        try:
            self.notifySocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.notifySocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.notifySocket.bind((self.host, self.notifyingPort))
            self.notifySocket.listen()

        except:
            print('Exception') 

        while True:
            
            client, address = self.notifySocket.accept()

            print(colors.GREEN + 'New client connected' + colors.ENDC + " " + str(address))
            threading.Thread(target = self.notifyClient, args = (client, address)).start()



    def notifyClient(self, client, address):
        print("Notifying the client: " + str(address))
        notUnique = 1
        while notUnique:
            data = client.recv(1024)
            userName = pickle.loads(data)

            if userName not in self.userDict.keys():
                self.userDict[userName] = client
                print(userName + " is added to the client dictionary. ")
                notUnique = 0
                data = 'You chose wisely.'
                self.sendNotification(client, data)
            else:
                data = 'Set another user name for gods sake!'
                self.sendNotification(client, data)

        #self.sendNotification(client, address)


    def createBoard(self, boardString):
        newBoard = Board(5, 5, boardString)
        return newBoard

    def attachUser(self, newUser, board):
        print("Attaching user")
        board.attach(newUser)
        greenUserName = colors.writeGreen(newUser.getName())
        greenBoardName = colors.writeGreen(board.getName())
        print(colors.writeYellow('User ') + greenUserName + colors.writeYellow(' is attached to the board ') + greenBoardName)
            
    def sendNotification(self, client, data):
        print(colors.YELLOW + 'Sending notification' + colors.ENDC)
        print(colors.GREEN + str(client) + colors.GREEN)
        #print(colors.BLUE + userName + colors.BLUE)
        notification = pickle.dumps(data)
        client.send(notification)
        print(colors.YELLOW + 'Notificiation sent' + colors.ENDC)
    
    def sendAttachNotification(self, user):

        client = self.attachedUsers[user]
        b = user.getBoard[0]

        boardName = b.getName()

        print(colors.YELLOW + 'Sending user attached notification' + colors.ENDC)
        data = 'You are attached to the board: ' + boardName
        notification = pickle.dumps(data)
        print('client:')
        print(client)
        client.send(notification)
        print(colors.YELLOW + 'User attached notificiation sent' + colors.ENDC)

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

if __name__ == "__main__":
    while True:
        try:
            host = 'localhost'
            listening_port = 8000
            notifying_port = 5000
            break
        except ValueError:
            pass

    Server(host, listening_port, notifying_port).startListening()