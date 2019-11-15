from Block import *
import pymunk

class DominoBlock(Block):
    def __init__(self, center):
        self.size = (4.8, 2.24)
        self.dominoMass = 0.08
        self.dominoCenter = center
        
        Block.__init__(self, self.dominoMass, self.dominoCenter, self.size)
        print('Domino Block is created at position ' + str(center) + '.')
