import pyglet
import pymunk
# http://www.pymunk.org/en/latest/pymunk.pyglet_util.html
from pymunk.pyglet_util import DrawOptions

window = pyglet.window.Window(1280, 720, "Pymunk Tester", resizable=False)
options = DrawOptions()


# Create space

space = pymunk.Space()
space.gravity = 0, 0
#space.gravity = 0, -100

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

mass = 1

radius = 30
# inner radius = 0
# outer radius = 30
circle_moment = pymunk.moment_for_circle(mass, 0, radius)
circle_body = pymunk.Body(mass, circle_moment)
circle_body.position = 300, 300
circle_shape = pymunk.Circle(circle_body, radius)


poly_shape = pymunk.Poly.create_box(None, size=(50, 50))
poly_moment = pymunk.moment_for_poly(mass, poly_shape.get_vertices())
poly_body = pymunk.Body(mass, poly_moment)
poly_shape.body = poly_body
poly_body.position = 450, 300

# (mass, a, b, radius)
segment_moment = pymunk.moment_for_segment(mass, (0,0), (0,400), 2)
segment_body = pymunk.Body(mass, segment_moment)
segment_shape = pymunk.Segment(segment_body, (0,0), (0,400), 2)
segment_body.position = 600, 300


space.add(circle_body, circle_shape, poly_body, poly_shape, segment_body, segment_shape)



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


