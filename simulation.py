from Ball import *
from BowlingBall import *
from TennisBall import *
from MarbleBall import *
from Block import *
from BookBlock import *
from DominoBlock import *
from Segment import *

import pymunk
import pyglet
from pymunk.pyglet_util import DrawOptions


window = pyglet.window.Window(1280, 720, "Pymunk Tester", resizable=False)
options = DrawOptions()
space = pymunk.Space()
space.gravity = 0, -100


#####

ball1 = BowlingBall(center = (400,600))
#ball1.getMass()
ball2 = TennisBall(center = (600,600))
ball3 = MarbleBall(center = (500, 600))

space.add(ball1, ball1.body, ball2, ball2.body, ball3, ball3.body)


#####

block1 = BookBlock(center = (800, 800))
block2 = DominoBlock(center = (900, 800))

space.add(block1, block1.body, block2, block2.body)


#####

mass = 1
# (mass, a, b, radius)
segment = Segment(mass, (600,300), (600,700), 2)
space.add(segment, segment.body)


#####


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