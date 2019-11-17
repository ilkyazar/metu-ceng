import pymunk
from Board import *

class User():
    def __init__(self, name):
        self.setName(name)
        self.boards = {}

    def addBoard(self, board):
        self.boards[board.boardName] = board

    def deleteBoard(self,board):
        self.boards.pop(board.boardName)
    
    def getBoard(self):
        return self.boards

    def setName(self, name):
        self.userName = name

    def notify(self, state):
        pass