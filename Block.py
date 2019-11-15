import pymunk
import random

class Block(pymunk.Poly):
    def __init__(self, mass, center, size):
        r = random.randint(0, 255)
        g = random.randint(0, 255)
        b = random.randint(0, 255)
        self.color = [r,g,b]

        poly_shape = pymunk.Poly.create_box(None, size=size)
        vertices = poly_shape.get_vertices()
        poly_moment = pymunk.moment_for_poly(mass, vertices)

        body = pymunk.Body(mass, poly_moment, pymunk.Body.DYNAMIC)
        poly_shape.body = body

        body.position = center

        pymunk.Poly.__init__(self, body, vertices, transform=None, radius=0)
