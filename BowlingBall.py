from Ball import *
import pymunk

class BowlingBall(Ball):
    def __init__(self, center, bouncingFactor = 1):
        self.bowlingMass = 50
        self.bowlingRadius = 10
        self.bowlingCenter = center
        
        self.bouncingFactor = bouncingFactor
        Ball.__init__(self, self.bowlingMass, self.bowlingRadius, self.bowlingCenter)
        print('Bowlingball created')


