from Ball import *
import pymunk

class TennisBall(Ball):
    def __init__(self, center, bouncingFactor = 1):
        #A typical tennis ball might have a mass of 0.57 kg and a radius of 6.5 cm.
        self.tennisMass = 0.57
        self.tennisRadius = 6.5
        self.tennisCenter = center
        
        self.bouncingFactor = bouncingFactor

        Ball.__init__(self, self.tennisMass, self.tennisRadius, self.tennisCenter)
        print('Tennis Ball is created at position ' + str(center) + '.')


