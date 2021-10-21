from .Block import *
import pymunk

class DominoBlock(Block):
    def __init__(self, center):
        size = (4.8, 2.24)
        dominoMass = 0.08
        dominoCenter = center
        
        Block.__init__(self, dominoMass, dominoCenter, size)
        print('Domino Block is created at position ' + str(center) + '.')

    def getType(self):
        return "dominoBlock"