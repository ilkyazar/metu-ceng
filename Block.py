import pymunk
import random

class Block(pymunk.Poly):
    def __init__(self, mass, center, size):

        self.setColor()
        self.setMass(mass)
        self.setCenter(center)
        self.setSize(size)
        self.setShape()
        self.setVertices()
        self.setBody()
        self.setPosition()              

        pymunk.Poly.__init__(self, self.blockBody, self.blockVertices, transform=None, radius=0)


    def setColor(self):
        r = random.randint(0, 255)
        g = random.randint(0, 255)
        b = random.randint(0, 255)
        self.color = [r,g,b]

    def setMass(self, mass):
        self.blockMass = mass

    def getMass(self):
        return self.blockMass

    def setCenter(self, center):
        self.blockCenter = center

    def setSize(self, size):
        self.blockSize = size

    def getSize(self):
        return self.blockSize

    def setShape(self):
        self.blockShape = pymunk.Poly.create_box(None, size=self.blockSize)

    def setVertices(self):
        self.blockVertices = self.blockShape.get_vertices()

    def setBody(self):
        poly_moment = pymunk.moment_for_poly(self.blockMass, self.blockVertices)
        self.blockBody = pymunk.Body(self.blockMass, poly_moment, pymunk.Body.DYNAMIC)
        self.blockShape.body = self.blockBody

    def setPosition(self):
        self.blockBody.position = self.blockCenter

    def getPosition(self):
        return self.blockBody.position
