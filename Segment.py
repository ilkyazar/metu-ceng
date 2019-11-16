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
        
        pymunk.Segment.__init__(self, self.segmentBody, self.p1, self.p2, self.segmentRadius)
    
    def setColor(self):
        r = random.randint(0, 255)
        g = random.randint(0, 255)
        b = random.randint(0, 255)
        self.color = [r,g,b]
    
    def setMass(self,mass):
        self.segmentMass = mass
    
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
    
    def setBody(self):
        segment_moment = pymunk.moment_for_segment(self.segmentMass, self.p1, self.p2, self.segmentRadius)
        self.segmentBody = pymunk.Body(self.segmentMass, segment_moment, pymunk.Body.DYNAMIC)
 