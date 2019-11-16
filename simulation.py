from Ball import *
from BowlingBall import *
from TennisBall import *
from MarbleBall import *
from Block import *
from BookBlock import *
from DominoBlock import *
from Segment import *
from RotatingSegment import *

import pymunk
import pyglet
from pymunk.pyglet_util import DrawOptions


window = pyglet.window.Window(1280, 720, "Pymunk Tester", resizable=False)
options = DrawOptions()
space = pymunk.Space()
space.gravity = 0, -50


#####

ball1 = BowlingBall(center = (250, 500))
#ball1.getMass()
ball2 = TennisBall(center = (70,60))
ball3 = MarbleBall(center = (90, 60))

space.add(ball1, ball1.body, ball2, ball2.body, ball3, ball3.body)


#####

block1 = BookBlock(center = (100, 200))
block2 = DominoBlock(center = (200, 200))

space.add(block1, block1.body, block2, block2.body)


#####

mass = 1
# (mass, a, b, radius)
segment = Segment(mass, (600,300), (600,700), 2)
space.add(segment, segment.body)


#####
rotation_center = (400,400)
rotation_center_body = pymunk.Body(body_type = pymunk.Body.STATIC) # 1
rotation_center_body.position = rotation_center

body = pymunk.Body(10, 10000) # 2
body.position = rotation_center
l1 = pymunk.Segment(body, (-150, 0), (150.0, 0.0), 10.0)

rotation_center_joint = pymunk.PinJoint(body, rotation_center_body, (0,0), (0,0)) # 3
space.add(l1, body, rotation_center_joint)

#####

segment2 = RotatingSegment(rotationCenter = (800,400), length = 100,  radius = 5.0)
print(segment2.a)
print(segment2.b)
print(segment2.getBody())
print(segment2.getShape().body)
#space.add(segment2.getShape(), segment2.getBody(), segment2.getJoint())
space.add(segment2, segment2.getBody(), segment2.getJoint())

@window.event
def on_draw():
    window.clear()
    space.debug_draw(options)   # DrawOptions


# Update the space in update method
def update(dt):
    space.step(dt)

if __name__ == "__main__":
    # Every 1.0/60 of a sec, update
    pyglet.clock.schedule_interval(update, 1.0/60)
    pyglet.app.run()