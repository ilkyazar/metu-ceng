from Ball import *
import pymunk

class BowlingBall(Ball):
    def __init__(self, center, bouncingFactor = 1):
        #A typical bowling ball might have a mass of 8 kg and a radius of 12 cm.
        self.bowlingMass = 8
        self.bowlingRadius = 12
        self.bowlingCenter = center
        
        self.bouncingFactor = bouncingFactor

        Ball.__init__(self, self.bowlingMass, self.bowlingRadius, self.bowlingCenter)
        print('Bowling Ball is created at position ' + str(center) + '.')


