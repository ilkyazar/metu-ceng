import pymunk
from Segment import *

class RotatingSegment(pymunk.Segment):
    def __init__(self, rotationCenter, length, radius, orientation = "horizontal"):
        self.setRotationCenter(rotationCenter)
        self.setLength(length)

        self.rotation_center_body = pymunk.Body(body_type = pymunk.Body.STATIC) #the static point where segment will rotate around
        self.rotation_center_body.position = self.rotationCenter

        self.setBody()
        self.setOrientation(orientation)
        self.setVertices(self.rotationCenter, self.length)
        self.setRadius(radius)
        self.setShape()

        pymunk.Segment.__init__(self, self.segmentBody, self.p1, self.p2, self.segmentRadius)

    def setRadius(self, radius):
        self.segmentRadius = radius
    
    def getRadius(self):
        return self.segmentRadius

    def setRotationCenter(self, rotationCenter):
        self.rotationCenter = rotationCenter
    
    def getRotationCenter(self):
        return self.rotationCenter
    
    def getPosition(self):
        return self.getRotationCenter()

    def setOrientation(self, orientation):
        self.orientation = orientation
    
    def getOrientation(self):
        return self.orientation

    def setLength(self, length):
        self.length = length
    
    def getLength(self):
        return self.length

    def setShape(self):
        self.segmentShape = pymunk.Segment(self.segmentBody, self.p1, self.p2, self.segmentRadius)

    def getShape(self):
        return self.segmentShape

    def getJoint(self):
        rotation_center_joint = pymunk.PinJoint(self.segmentBody, self.rotation_center_body, (0,0), (0,0))
        return rotation_center_joint
    
    def setBody(self):
        self.segmentBody = pymunk.Body(10, 10000)
        self.segmentBody.position = self.rotationCenter

    def getBody(self):
        return self.segmentBody

    def setVertices(self, rotationCenter, length):
        if self.orientation == "horizontal":
            self.p1 = (-(self.length/2),0)
            self.p2 = ((self.length/2), 0)
        else:
            self.p1 = (0, -(self.length/2))
            self.p2 = (0, (self.length/2))
    
    def getType(self):
        return "rotatingSegment"