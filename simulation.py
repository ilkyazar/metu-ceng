from Ball import *
from BowlingBall import *
from TennisBall import *
from MarbleBall import *
from Block import *
from BookBlock import *
from DominoBlock import *
from Segment import *
from Connector import * 

from pygame.locals import *
import pymunk.pygame_util

import pymunk
import pyglet
import pygame
from pymunk.pyglet_util import DrawOptions
from pymunk.vec2d import Vec2d

pymunk.pygame_util.positive_y_is_up = False

space = pymunk.Space()
space.gravity = 0, -50

pygame.init()
screen = pygame.display.set_mode((1200, 600))
options = pymunk.pygame_util.DrawOptions(screen)

clock = pygame.time.Clock()
font = pygame.font.Font(None, 24)

#####

ball1 = BowlingBall(center = (250, 500))
#ball1.getMass()
ball2 = TennisBall(center = (70,60))
ball3 = MarbleBall(center = (90, 60))

space.add(ball2, ball2.body, ball3, ball3.body)

space.add(ball1.body, ball1.getShape())

connectorBall2_3 = Connector(ball2.getShape(), ball3.getShape())
space.add(connectorBall2_3.getPinJoint())

#####

block1 = BookBlock(center = (100, 200))
block2 = DominoBlock(center = (200, 200))

space.add(block1, block1.body, block2, block2.body)


#####

mass = 1
# (mass, a, b, radius)
segment = Segment(mass, (-800,-100), (2000,-100), 200)
segment.body.body_type = pymunk.Body.STATIC
space.add(segment, segment.body)


#####
rotation_center_body = pymunk.Body(body_type = pymunk.Body.STATIC) # 1
rotation_center_body.position = (300, 300)

body = pymunk.Body(10, 10000) # 2
body.position = (300, 300)
l1 = pymunk.Segment(body, (-150, 0), (150.0, 0.0), 5.0)

rotation_center_joint = pymunk.PinJoint(body, rotation_center_body, (0,0), (0,0)) # 3

space.add(l1, body, rotation_center_joint)
#####

mouse_joint = None
mouse_body = pymunk.Body(body_type=pymunk.Body.KINEMATIC)

while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            exit()
        elif event.type == KEYDOWN and event.key == K_ESCAPE:
            exit()
        elif event.type == MOUSEBUTTONDOWN:
            if mouse_joint != None:
                space.remove(mouse_joint)
                mouse_joint = None
                print("mouse joint is not none")

            p = Vec2d(event.pos)
            print(p)
            hit = space.point_query_nearest(p, 5, pymunk.ShapeFilter())
            print(hit)

            if (hit != None):
                print(hit.shape.body.body_type)

            if hit != None and hit.shape.body.body_type == pymunk.Body.DYNAMIC:
                shape = hit.shape
                # Use the closest point on the surface if the click is outside 
                # of the shape.
                if hit.distance > 0:
                    nearest = hit.point 
                else:
                    nearest = p
                mouse_joint = pymunk.PivotJoint(mouse_body, shape.body, 
                    (0,0), shape.body.world_to_local(nearest))
                mouse_joint.max_force = 50000
                mouse_joint.error_bias = (1-0.15) ** 60
                space.add(mouse_joint)
                print("hit is not none and body is dynamic")

            print("mouse button down")

        elif event.type == MOUSEBUTTONUP:
            if mouse_joint != None:
                space.remove(mouse_joint)
                mouse_joint = None
            print("mouse button up")

    screen.fill(pygame.color.THECOLORS["white"])


    mouse_pos = pygame.mouse.get_pos()

    mouse_body.position = mouse_pos

    space.step(1./60)
    
    space.debug_draw(options)
    pygame.display.flip()

    clock.tick(60)
    pygame.display.set_caption("fps: " + str(clock.get_fps()))