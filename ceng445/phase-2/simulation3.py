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
from User import *

from pygame.locals import *
import pymunk.pygame_util

import pymunk
import pygame
from pymunk.vec2d import Vec2d

user0 = User("admin")
board1 = Board(1200, 600, 'simulation_3')
user0.addBoard(board1)
board1.start((1./60.0), 60, 1000)

#create shapes
ball1 = BowlingBall(center = (250, 500))
ball2 = TennisBall(center = (70,60))
ball3 = MarbleBall(center = (90, 60))
block1 = BookBlock(center = (100, 200))
block2 = DominoBlock(center = (200, 200))
segment1 = RotatingSegment(rotationCenter = (800,400), length = 100,  radius = 5.0)

#add shapes to board
board1.addShape(ball1, ball1.getBody())
#board1.addShape(ball2, ball2.getBody())
#board1.addShape(ball3, ball3.getBody())
board1.addShape(block1, block1.getBody())
board1.addShape(block2, block2.getBody())
board1.addShape(segment1, segment1.getBody())

#add users
user1 = User("ilkyazar") 
user2 = User("dilsada")
board1.attach(user1)
board1.attach(user2)
board1.attach(user0)

#visualize
pymunk.pygame_util.positive_y_is_up = False
pygame.init()
clock = pygame.time.Clock()
font = pygame.font.Font(None, 24)
board1.screen.fill(pygame.color.THECOLORS["white"])

#save file
board1.save("test3.json")

#show current state
print ("Current State: " + str(board1.state(update = False)))

def notifyUser():
    for user in board1.users:
        board1.users[user].notify(board1.state(update = False))

def getUserText():
    user_result = ""
    for user in board1.users.keys():
        user_result += user
        user_result += ' '
    text = "Users who are watching: " + user_result
    
    #list() functionality of the Board class
    board_result = ""
    for boardName, board in user0.getBoard().items():
        board_result += boardName
        board_result += ' '
    text += "-- Current available boards: " + board_result

    return font.render(text, 1, pygame.color.THECOLORS["black"])

s=0
while s < board1.number_of_steps:
    #moveShape
    if (ball1.getPosition().x == 250):
        offset = (1,0)
        #detach user
        board1.detach(user2)
        #try to detach a user that is already detached
        board1.detach(user2)

    elif (ball1.getPosition().x == 500):
        offset = (-1,0)
        board1.attach(user2)
        #try to attach a user that is already attached
        board1.attach(user2)
        
    board1.moveShape(ball1, offset)

    #current boards and attached users
    user_txt = getUserText()

    board1.screen.fill(pygame.color.THECOLORS["white"])
    board1.screen.blit(user_txt, (5, board1.screen.get_height() - 50))
    board1.updateSpace()
    options = pymunk.pygame_util.DrawOptions(board1.screen)
    board1.space.debug_draw(options)
    pygame.display.flip()
    clock.tick(board1.k)
    
    s += 1





