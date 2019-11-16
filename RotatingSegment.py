import pymunk
from Segment import *

class RotatingSegment(pymunk.Segment):
    def __init__(self, rotationCenter, length, radius):
        self.setRotationCenter(rotationCenter)
        self.setLength(length)

        self.rotation_center_body = pymunk.Body(body_type = pymunk.Body.STATIC) #the static point where segment will rotate around
        self.rotation_center_body.position = self.rotationCenter

        self.setBody()
        self.setVertices(self.rotationCenter, self.length)
        self.setRadius(radius)
        self.setShape()

        pymunk.Segment.__init__(self, self.segmentBody, self.p1, self.p2, self.segmentRadius)

    def setRadius(self, radius):
        self.segmentRadius = radius

    def setRotationCenter(self, rotationCenter):
        self.rotationCenter = rotationCenter

    def setLength(self, length):
        self.length = length

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
        self.p1 = (-(self.length/2),0)
        self.p2 = ((self.length/2), 0)
