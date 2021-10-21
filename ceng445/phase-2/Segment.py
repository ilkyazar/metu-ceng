import pymunk
import random

class Segment(pymunk.Segment):
    def __init__(self, mass, p1, p2, radius):
        self.setColor()
        self.setMass(mass)
        self.setVertice1(p1)
        self.setVertice2(p2)
        self.setRadius(radius)
        self.setBody()
        self.setShape()

        pymunk.Segment.__init__(self, self.segmentBody, self.p1, self.p2, self.segmentRadius)
    
    def setShape(self):
        self.segmentShape = pymunk.Segment(self.segmentBody, self.p1, self.p2, self.segmentRadius)

    def getShape(self):
        return self.segmentShape

    def setColor(self):
        r = random.randint(0, 255)
        g = random.randint(0, 255)
        b = random.randint(0, 255)
        self.color = [r,g,b]
    
    def setMass(self,mass):
        self.segmentMass = mass
    
    def getMass(self):
        return self.segmentMass
    
    def setVertice1(self,p1):
        self.p1 = p1
    
    def setVertice2(self,p2):
        self.p2 = p2

    def getVertice1(self):
        return self.p1
    
    def getVertice2(self):
        return self.p2
    
    def setRadius(self, radius):
        self.segmentRadius = radius

    def getRadius(self):
        return self.segmentRadius
    
    def getBody(self):
        return self.segmentBody

    def setBody(self):
        segment_moment = pymunk.moment_for_segment(self.segmentMass, self.p1, self.p2, self.segmentRadius)
        self.segmentBody = pymunk.Body(self.segmentMass, segment_moment, pymunk.Body.DYNAMIC)
    
    def getType(self):
        return "segment"

    def getPosition(self):
        pos = self.p1
        pos[0] = (self.p1[0] + self.p2[0]) / 2
        pos[1] = (self.p1[1] + self.p2[1]) / 2
        return pos