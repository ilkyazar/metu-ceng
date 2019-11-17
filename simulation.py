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
import pygame
from pymunk.vec2d import Vec2d


pymunk.pygame_util.positive_y_is_up = False
pygame.init()
clock = pygame.time.Clock()
font = pygame.font.Font(None, 24)

board1 = Board()
board1.start((1./60.0), 60, 1000)
board1.load('test2.json')

help_txt = font.render(
    "Move objects with left click. Toggle connections with right click. Have fun! :)", 
    1, pygame.color.THECOLORS["black"])


options = pymunk.pygame_util.DrawOptions(board1.screen)
mouse_joint = None
mouse_body = pymunk.Body(body_type=pymunk.Body.KINEMATIC)

# containers
box_size = 200
w = board1.screen.get_width()
h = board1.screen.get_height()
for i in range(4):
    sw = pymunk.Segment(board1.space.static_body, 
        (0, i*box_size), (w, i* box_size), 1)
    sw.friction = 1
    sw.elasticity = 1
    sh = pymunk.Segment(board1.space.static_body, 
        (i*box_size, 0), (i*box_size, h-box_size), 1)
    sh.friction = 1
    sh.elasticity = 1
    board1.space.add(sw, sh)

toggle = True
ball1 = board1.getShapeWithID("1")
ball2 = board1.getShapeWithID("2")
ball3 = board1.getShapeWithID("3")
ball4 = board1.getShapeWithID("4")

while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            exit()
        
        elif event.type == MOUSEBUTTONDOWN and event.button == 3:
            print("Event: Mouse button down, clicked right")
            
            if(toggle):
                #Connect and disconnect functionalities
                board1.connect(ball3, ball4)
                board1.disconnectShapes(ball1, ball2)
            else:
                board1.connect(ball1, ball2)
                board1.disconnectShapes(ball3, ball4)
            
            toggle = not toggle

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