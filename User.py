import pymunk

class User():
    def __init__(self, name):
        self.setName(name)

    def setName(self, name):
        self.userName = name

    def notify(self, state):
        pass