import pymunk
import random

class Ball(pymunk.Circle):
    def __init__(self, mass, radius, center):
        
        self.setColor()
        self.setRadius(radius)
        self.setMass(mass)
        self.setBody()
        self.setPosition(center)              
        
        pymunk.Circle.__init__(self, self.ballBody, self.ballRadius)

    def setRadius(self, radius):
        self.ballRadius = radius

    def getRadius(self):
        return self.ballRadius

    def setMass(self, mass):
        self.ballMass = mass

    def getMass(self):
        return self.ballMass

    def setColor(self):
        r = random.randint(0, 255)
        g = random.randint(0, 255)
        b = random.randint(0, 255)
        self.color = [r,g,b]

    def setBody(self):
        circle_moment = pymunk.moment_for_circle(self.ballMass, 0, self.ballRadius)
        self.ballBody = pymunk.Body(self.ballMass, circle_moment, pymunk.Body.DYNAMIC)

    def setPosition(self, center):
        self.ballBody.position = center

    def getPosition(self):
        return self.ballBody.position