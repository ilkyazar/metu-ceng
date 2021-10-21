# example for starting simulation on server

# A pymunk example writing shapes as text file:

import sys
import matplotlib.pyplot as plt
from matplotlib import animation
import pymunk
from pymunk.vec2d import Vec2d
import pymunk.matplotlib_util

class SpaceDrawAsText(pymunk.SpaceDebugDrawOptions):
	def __init__(self):
		super().__init__()

	def draw_circle(self, *args):
		angle = args[1]
		x = args[0].x
		y = args[0].y
		radius = args[2]
		print('circle', x, y, radius, angle)

	def draw_segment(self, *args):
		x1, y1 = args[0].x, args[0].y
		x2, y2 = args[1].x, args[1].y
		print('line', x1, y1, x2, y2)

	def draw_fat_segment(self, *args):
		x1, y1 = args[0].x, args[0].y
		x2, y2 = args[1].x, args[1].y
		radius = args[2]
		print('segment', x1, y1, x2, y2, radius)

	def draw_dot(self, *args):
		x = args[1].x
		y = args[1].y
		width = args[0]
		print('dot', x, y, width)

def setup_space():
    space = pymunk.Space()
    space.gravity = 0,-9820
    space.damping = 0.99
    return space

def setup_balls(space):
    width = 600
    height = 600
    for x in range(-100,150,50):
        x += width / 2
        offset_y = height/2
        mass = 10
        radius = 25
        moment = pymunk.moment_for_circle(mass, 0, radius, (0,0))
        body = pymunk.Body(mass, moment)
        body.position = x, -125+offset_y
        body.start_position = Vec2d(body.position)
        shape = pymunk.Circle(body, radius)
        shape.elasticity = 0.9999999
        space.add(body, shape)
        pj = pymunk.PinJoint(space.static_body, body, (x, 125+offset_y), (0,0))
        space.add(pj)


space = setup_space()
setup_balls(space)

#o = pymunk.matplotlib_util.DrawOptions(ax)
o = SpaceDrawAsText()


space.shapes[1].body.apply_impulse_at_local_point((-12000,0))
    
def init():
    space.debug_draw(o)
    return []

def animate(dt):
    #we run the animation with half speed intentionally to make it a little nicer to look at
    for x in range(10):
        space.step(1/50/10/2)
    space.debug_draw(o)
    return []

print('frame',0)
init()
for frame in range(105):
	print('frame',frame +1)
	animate(10)

print('end')