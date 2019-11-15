import pymunk
import random

class Segment(pymunk.Segment):
    def __init__(self, mass, p1, p2, radius):
        r = random.randint(0, 255)
        g = random.randint(0, 255)
        b = random.randint(0, 255)
        self.color = [r,g,b]

        segment_moment = pymunk.moment_for_segment(mass, p1, p2, radius)
        body = pymunk.Body(mass, segment_moment, pymunk.Body.DYNAMIC)
        #could also be STATIC
        pymunk.Segment.__init__(self, body, p1, p2, radius)