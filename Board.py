import pymunk
import pygame
import json
import copy
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
from pymunk.vec2d import Vec2d
from colors import colors

class Board():
    def __init__(self, x=0, y=0, name=''):
        self.setName(name)
        self.setSpace()
        self.setScreen(x, y)
        self.allShapes = {}
        self.users = {}
        self.connectors = {}
        self.id_counter = 1

    def setName(self, name):
        self.boardName = name

    def getName(self):
        return self.boardName

    def setScreen(self, x, y):
        self.x = x
        self.y = y
        self.screen = pygame.display.set_mode((x, y))

        print(colors.writeBlue('A new board with ') + colors.writeGreen(str(self.x) + ' ') + colors.writeGreen(str(self.y) + ' ') + colors.writeBlue('dimensions is created.'))

    def setSpace(self):
        self.space = pymunk.Space()
        self.space.gravity = 0, -9820
        self.space.damping = 0.99
    
    def setGravity(self, gravity):
        self.space.gravity = gravity
    
    def updateSpace(self):
        self.space.step(self.step_size)

    def addShape(self, shape, offset=0):
        body = shape.getBody()
        self.space.add(shape)
        self.space.add(body)
        new_id = self.id_counter
        self.id_counter += 1
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
        center = (pos[0]+offset[0], pos[1]+offset[1])
        shape.setPosition(center)

    def connect(self, shape1, shape2, connectorClass = None):
        # Reminder: Add other connector classes as if checks later
        newConnector = Connector(shape1.getBody(), shape2.getBody())
        self.space.add(newConnector.getPinJoint())

        connector_id = self.id_counter
        self.id_counter += 1
        self.connectors[str(connector_id)] = [newConnector, shape1, shape2]
        print("New connection established between " + str(shape1) + " and " + str(shape2))

    def disconnect(self, connector_id):
        connector = (self.connectors.pop(connector_id))[0]
        self.space.remove(connector.getPinJoint())
    
    def disconnectShapes(self, shape1, shape2):
        for key,value in self.connectors.items():
            if(shape1 in value and shape2 in value):
                self.disconnect(key)
                print("Connection between " + str(shape1) + " and " + str(shape2) + " is removed.")
                break

    def pick(self, x, y):
        '''
        picklist = []
        for shape in self.allShapes:
            if (self.allShapes[shape].getPosition()[0] == x and self.allShapes[shape].getPosition()[1] == y):
                picklist.append(self.allShapes[shape])
        for p in range(len(picklist)): 
            print(str(picklist[p]) + " \n")
        '''
        p = Vec2d(x, y)
        hit1 = self.space.point_query_nearest(p, 5, pymunk.ShapeFilter())

        print("\n pick(" + str(x) + ", " + str(y) + ") result: \n" )
        if hit1 != None:
            print(hit1.shape) 
        print("\n")  
        return hit1

    def getShapeWithID(self, id):
        #for test purposes
        return self.allShapes[id]
        
    def attach(self, user):
        if(user.userName not in self.users.keys()):
            self.users[user.userName] = user

        else:
            print (colors.RED + 'User already attached!' + colors.ENDC)

    def detach(self, user):
        if(user.userName in self.users.keys()):
            self.users.pop(user.userName)
        else:
            print (colors.RED + 'User is not attached, you can not detach it!' + colors.ENDC)

    def list(self):
        #an admin user will store all boards
        #dont forget to use user classes methods (addboard, deleteboard etc.) in simulation3
        for user in self.users:
            if self.users[user].getName() == "admin":
                boards = self.users[user].boards
                for board in boards:
                    print("All boards in the system: \n")
                    print(boards[board].boardName + "\n")
    
    #(step_size = (1./60.0), k = clock.tick, number_of_steps = game length)
    def start(self, step_size, k, number_of_steps):
        self.step_size = step_size
        self.number_of_steps = number_of_steps
        self.k = k
        self.allShapes = {}
        self.users = {}

    def save(self, file):
        content = {}
        content['boardName'] = self.boardName
        content['x'] = self.x
        content['y'] = self.y
        content['gravity'] = list(self.space.gravity)

        content['balls'] = []
        content['blocks'] = []
        content['segments'] = []
        content['connectors'] = []

        for key, value in self.allShapes.items():
            
            shapeType = value.getType()

            if(shapeType in ("bowlingBall", "marbleBall", "tennisBall")):
                newBall = {
                    "id": key,
                    "type": shapeType,
                    "center": list(value.getPosition())
                }
                content['balls'].append(newBall)
            
            elif(shapeType in ("bookBlock", "dominoBlock")):
                newBlock = {
                    "id": key,
                    "type": shapeType,
                    "center": list(value.getPosition())
                }
                content['blocks'].append(newBlock)
            
            elif(shapeType == "segment"):
                newSegment = {
                    "id": key,
                    "type": shapeType,
                    "mass": value.getMass(),
                    "p1": list(value.getVertice1()),
                    "p2": list(value.getVertice2()),
                    "radius": value.getRadius()
                }
                content['segments'].append(newSegment)

            elif(shapeType == "rotatingSegment"):
                newSegment = {
                    "id": key,
                    "type": shapeType,
                    "rotationCenter": list(value.getRotationCenter()),
                    "length": value.getLength(),
                    "radius": value.getRadius(),
                    "orientation": value.getOrientation()
                }
                content['segments'].append(newSegment)
            
            else:
                print("INVALID SHAPE TYPE TO SAVE")
        
        with open(file, 'w') as outfile:
            json.dump(content, outfile)

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
  
        for block in content['blocks']:
            if block['type'] == 'bookBlock':
                newBlock = BookBlock(block['center'])    
            else:
                newBlock = DominoBlock(block['center'])
            
            self.addShape(newBlock, newBlock.body)

        for segment in content['segments']:
            if segment['type'] == 'segment':
                newSegment = Segment(segment['mass'], segment['p1'], segment['p2'], segment['radius'])
                self.addShape(newSegment, newSegment.body)
                
            else:
                newSegment = RotatingSegment(segment['rotationCenter'], segment['length'], segment['radius'], segment['orientation'])
                self.addShape(newSegment, newSegment.getBody())
                self.addJoint(newSegment.getJoint())

        for connector in content['connectors']:
            shape1_id = connector['shape1_id']
            shape2_id = connector['shape2_id']
            shape1 = self.allShapes[shape1_id]
            shape2 = self.allShapes[shape2_id] 
            self.connect(shape1, shape2)
        
        print(colors.writeBold("JSON file loaded successfully from " + file))

    def state(self, update = False):
        state = []
        for key,value in self.allShapes.items():
            item = [value, key, value.getPosition()]
            state.append(item)

        return state
        # visualization vs simulation, how will we implement the difference for further phases