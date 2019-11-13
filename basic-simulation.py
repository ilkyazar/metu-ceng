import pymunk
import time

space = pymunk.Space()
space.gravity = 0, -100

body = pymunk.Body(1, 1666)
# Rigid bodies hold physical properties of objects (mass position, rotation, velocity, etc.)
# We need to create a shape for a body

body.position = 50, 100

space.add(body)

while True:
    space.step(0.02) # 50 frames per sec
    print(body.position)
    time.sleep(0.5)