import pymunk
import random

class Ball(pymunk.Circle):
    def __init__(self, mass, radius, center):
        r = random.randint(0, 255)
        g = random.randint(0, 255)
        b = random.randint(0, 255)
        self.color = [r,g,b]

        circle_moment = pymunk.moment_for_circle(mass, 0, radius)
        body = pymunk.Body(mass, circle_moment, pymunk.Body.DYNAMIC)
        body.position = center

        pymunk.Circle.__init__(self, body, radius)



