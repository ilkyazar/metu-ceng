from Ball import *
import pymunk

class MarbleBall(Ball):
    def __init__(self, center, bouncingFactor = 1):
        marbleMass = 0.05
        marbleRadius = 3
        marbleCenter = center
        
        self.bouncingFactor = bouncingFactor

        Ball.__init__(self, marbleMass, marbleRadius, marbleCenter)
        print('Marble Ball is created at position ' + str(center) + '.')

    def getType(self):
        return "marbleBall"
