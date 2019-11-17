import pymunk
import pygame
import json
from Ball import *
from BowlingBall import *
from MarbleBall import *
from TennisBall import *
from Block import *
from BookBlock import *
from DominoBlock import *
from Segment import *
from RotatingSegment import *
from Connector import * 

class Board():
    def __init__(self, x=0, y=0, name=''):
        self.setName(name)
        self.setSpace()
        self.setScreen(x, y)
        self.allShapes = {}
        self.users = {}

    def setName(self, name):
        self.boardName = name

    def getName(self):
        return self.boardName

    def setScreen(self, x, y):
        self.screen = pygame.display.set_mode((x, y))

    def setSpace(self):
        self.space = pymunk.Space()
        self.space.gravity = 0, 0
    
    def setGravity(self, gravity):
        self.space.gravity = gravity
    
    def updateSpace(self):
        #self.lastUpdated = self.allShapes
        self.space.step(self.step_size)

    def addShape(self, shape, body, offset=0):
        self.space.add(shape)
        self.space.add(body)
        if self.allShapes == {}:
            new_id = 1
        else:
            new_id = len(self.allShapes) + 1
        self.allShapes[str(new_id)] = shape

    def addJoint(self, joint):
        self.space.add(joint)

    def removeShape(self, shape, offset=500):  
        # Reminder: If two shapes of the connector is removed, remove the connector too
        for key, value in self.allShapes.items(): 
            print("shape: "+ str(shape))
            print(value)

            if (shape.getBody().position.x <= value.getBody().position.x + offset and shape.getBody().position.y <= value.getBody().position.y + offset) or (shape.getBody().position.x >= value.getBody().position.x - offset and shape.getBody().position.y <= value.getBody().position.y + offset):
                shape_id = key
                self.allShapes.pop(shape_id)
                self.space.remove(shape)
                print("Shape is removed from the space with id " + str(shape_id))
                return 
            else:
                print("INVALID SHAPE TO REMOVE")
                return 

    def removeShapeWithID(self, shape_id):
        shape = self.allShapes.pop[shape_id]
        self.space.remove(shape)

    def moveShape(self, shape, offset):
        pos = shape.getPosition()
        center = (pos[0]+offset[0], pos[0]+offset[1])
        shape.setPosition(center)

    def connect(self, shape1, shape2, connectorClass = None):
        # Reminder: Add other connector classes as if checks 
        newConnector = Connector(shape1.getBody(), shape2.getBody())
        self.space.add(newConnector.getPinJoint())
        return newConnector

    def disconnect(self, connector_id):
        connector = self.allShapes.pop[connector_id]
        self.space.remove(connector)

    def pick(self, x, y):
        pass

    def attach(self, user):
        if(user.userName not in self.users.keys()):
            self.users[user.userName] = user
        else:
            print ('User already attached!')

    def detach(self, user):
        if(user.userName in self.users.keys()):
            self.users.pop(user.userName)
        else:
            print ('User is not attached, you can not detach it!')

    def list(self):
        #an admin user will store all boards
        #dont forget to use user classes methods (addboard, deleteboard etc.) in simulation3
        pass
    
    #(1./60.0)
    def start(self, step_size, k, number_of_steps):
        self.step_size = step_size
        self.number_of_steps = number_of_steps
        self.k = k
        self.allShapes = {}
        self.users = {}


    def save(self, file):
        pass

    def load(self, file):
        with open(file) as json_file:
            content = json.load(json_file)
        self.setName(content['boardName'])
        self.setScreen(content['x'], content['y'])
        self.setGravity(content['gravity'])

        for ball in content['balls']:
            if ball['type'] == 'bowlingBall':
                newBall = BowlingBall(ball['center'])
            elif ball['type'] == 'marbleBall':
                newBall = MarbleBall(ball['center'])
            else:
                newBall = TennisBall(ball['center'])

            self.addShape(newBall, newBall.body)
            #self.allShapes[ball['id']] = newBall

        for block in content['blocks']:
            if block['type'] == 'bookBlock':
                newBlock = BookBlock(block['center'])
            else:
                newBlock = DominoBlock(block['center'])

            self.addShape(newBlock, newBlock.body)
            #self.allShapes[block['id']] = newBlock
        
        for segment in content['segments']:
            if segment['type'] == 'segment':
                newSegment = Segment(segment['mass'], segment['p1'], segment['p2'], segment['radius'])
                self.addShape(newSegment, newSegment.body)
                
            else:
                newSegment = RotatingSegment(segment['rotationCenter'], segment['length'], segment['radius'], segment['orientation'])
                self.addShape(newSegment, newSegment.getBody())
                self.addJoint(newSegment.getJoint())

            #self.allShapes[segment['id']] = newSegment
        
        for connector in content['connectors']:
            shape1_id = connector['shape1_id']
            shape2_id = connector['shape2_id']
            shape1 = self.allShapes[shape1_id]
            shape2 = self.allShapes[shape2_id] 
            newConnector = self.connect(shape1, shape2)
            #self.allShapes[connector['id']] = newConnector

    def state(self, update):
        #allShapes with their positions
        # visualization vs simulation, how will we implement the difference for further phases
        pass