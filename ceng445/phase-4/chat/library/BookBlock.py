from .Block import *
import pymunk

class BookBlock(Block):
    def __init__(self, center):
        size = (25, 20)
        bookMass = 0.5
        bookCenter = center
        
        Block.__init__(self, bookMass, bookCenter, size)
        print('Book Block is created at position ' + str(center) + '.')

    def getType(self):
        return "bookBlock"