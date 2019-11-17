from Ball import *
from BowlingBall import *
from TennisBall import *
from MarbleBall import *
from Block import *
from BookBlock import *
from DominoBlock import *
from Segment import *
from RotatingSegment import *
from Connector import * 
from Board import * 

from pygame.locals import *
import pymunk.pygame_util

import pymunk
#import pyglet
import pygame
#from pymunk.pyglet_util import DrawOptions
from pymunk.vec2d import Vec2d


pymunk.pygame_util.positive_y_is_up = False
pygame.init()
clock = pygame.time.Clock()
font = pygame.font.Font(None, 24)


board1 = Board()
board1.start((1./60.0), 60, 1000)
board1.load('test1.json')
#board1.load('test3.json')
help_txt = font.render(
    "Move objects with left click. Remove objects with right click. Have fun! :)", 
    1, pygame.color.THECOLORS["darkgray"])


options = pymunk.pygame_util.DrawOptions(board1.screen)

mouse_joint = None
mouse_body = pymunk.Body(body_type=pymunk.Body.KINEMATIC)


while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            exit()
        elif event.type == KEYDOWN and event.key == K_ESCAPE:
            exit()
        elif event.type == MOUSEBUTTONDOWN and event.button == 3:
            print("Event: Mouse button down, clicked right")
            
            if mouse_joint != None:
                board1.space.remove(mouse_joint)
                mouse_joint = None

            p = Vec2d(event.pos)
            print("Event position: " + str(p))
            hit = board1.space.point_query_nearest(p, 5, pymunk.ShapeFilter())

            if hit != None and hit.shape.body.body_type == pymunk.Body.DYNAMIC:
                shape = hit.shape
                board1.removeShape(shape)

        elif event.type == MOUSEBUTTONDOWN and event.button == 1:
            print("Event: Mouse button down, clicked left")

            if mouse_joint != None:
                board1.space.remove(mouse_joint)
                mouse_joint = None

            p = Vec2d(event.pos)
            print("Event position: " + str(p))
            hit = board1.space.point_query_nearest(p, 5, pymunk.ShapeFilter())

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

                board1.space.add(mouse_joint)


        elif event.type == MOUSEBUTTONUP:
            if mouse_joint != None:
                board1.space.remove(mouse_joint)
                mouse_joint = None
            print("Event: Mouse button up")

    board1.screen.fill(pygame.color.THECOLORS["white"])

    board1.screen.blit(help_txt, (5, board1.screen.get_height() - 20))

    mouse_pos = pygame.mouse.get_pos()

    mouse_body.position = mouse_pos

    board1.updateSpace()
    board1.space.debug_draw(options)
    pygame.display.flip()

    clock.tick(60)