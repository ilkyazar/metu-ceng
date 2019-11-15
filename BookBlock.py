from Block import *
import pymunk

class BookBlock(Block):
    def __init__(self, center):
        self.size = (25, 20)
        self.bookMass = 0.5
        self.bookCenter = center
        
        Block.__init__(self, self.bookMass, self.bookCenter, self.size)
        print('Book Block is created at position ' + str(center) + '.')
