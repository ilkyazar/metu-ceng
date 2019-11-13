import pyglet
import pymunk
# http://www.pymunk.org/en/latest/pymunk.pyglet_util.html
from pymunk.pyglet_util import DrawOptions

window = pyglet.window.Window(1280, 720, "Pymunk Tester", resizable=False)
options = DrawOptions()


# Create space

space = pymunk.Space()
space.gravity = 0, -100


# Dynamic bodies are affected from gravity and collisions
# (ball, player, enemy)
# They are the default body types
# ->>> Go to Definiton for more --Body


# Kinematic bodies are not affected by gravity but can be moved 
# (platforms, doors)


# Static bodies are not affected by gravity and can NOT be moved
# (ground, building)


# Rigid bodies hold physical properties of objects (mass position, rotation, velocity, etc.)
# We need to create a shape for a body

# Create a shape
poly = pymunk.Poly.create_box(None, size=(100, 100))

# Calculate the moment of inertia for a solid ploygon shape
# First parameter = mass
# Second parameter = vertices
moment = pymunk.moment_for_poly(10, poly.get_vertices())
#print(moment)

# First parameter = mass
# second parameter = inertia
# third parameter = body type
dynamic_body = pymunk.Body(1, moment, pymunk.Body.DYNAMIC)
static_body = pymunk.Body(body_type = pymunk.Body.STATIC)
kinetic_body = pymunk.Body(body_type = pymunk.Body.KINEMATIC)


dynamic_body.position = 600, 600
poly.body = dynamic_body
space.add(dynamic_body, poly)


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


