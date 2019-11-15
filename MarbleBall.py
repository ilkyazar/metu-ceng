from Ball import *
import pymunk

class MarbleBall(Ball):
    def __init__(self, center, bouncingFactor = 1):
        self.marbleMass = 0.05
        self.marbleRadius = 3
        self.marbleCenter = center
        
        self.bouncingFactor = bouncingFactor

        Ball.__init__(self, self.marbleMass, self.marbleRadius, self.marbleCenter)
        print('Marble Ball is created at position ' + str(center) + '.')


