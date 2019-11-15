import pymunk

class Ball(pymunk.Circle):
    def __init__(self, mass, radius, center, color = [0,0,0]):
        self.color = color

        circle_moment = pymunk.moment_for_circle(mass, 0, radius)
        self.body = pymunk.Body(mass, circle_moment)

        pymunk.Circle.__init__(self, self.body, radius, center)
        #pymunk.Circle.color = color
        print('Ball created')



#ball = Ball(mass = 1, radius = 10, center = (0,0))