from Ball import *
import pymunk

class BowlingBall(Ball):
    def __init__(self, center, bouncingFactor = 1):
        bowlingMass = 8
        bowlingRadius = 12
        bowlingCenter = center
        
        self.bouncingFactor = bouncingFactor

        Ball.__init__(self, bowlingMass, bowlingRadius, bowlingCenter)
        print('Bowling Ball is created at position ' + str(center) + '.')


