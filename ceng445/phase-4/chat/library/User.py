import pymunk
from .Board import *

class User():
    def __init__(self, name):
        self.setName(name)
        self.boards = {}
        self.currentState = {}

    def addBoard(self, board):
        self.boards[board.boardName] = board

    def deleteBoard(self,board):
        self.boards.pop(board.boardName)
    
    def getBoard(self):
        return self.boards

    def setName(self, name):
        self.userName = name

    def getName(self):
        return self.userName

    def notify(self, state):
        self.currentState = state
        print("User " + self.getName() + " is notified with state: " + str(state))