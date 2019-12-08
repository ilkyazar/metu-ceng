# example for displaying on client

# A code that gets that output and displays in matplotlib:

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.lines as mlines
import matplotlib.patches as mpatches
from matplotlib import animation
from matplotlib.collections import PatchCollection

import sys

inp = sys.stdin

frames = []
shapes = []
while True:
	line = inp.readline()
	if line == '':
		if shapes != [] : 
			frames.append(tuple(shapes))
		break
	
	vals = line.split(' ')
	if vals[0].upper() == 'END':
		frames.append(tuple(shapes))
		break
	elif vals[0].upper() == 'FRAME':
		frames.append(tuple(shapes))
		shapes = []
	elif vals[0] == 'circle' or vals[0] == 'dot':
		x, y, r = float(vals[1]), float(vals[2]), float(vals[3])
		shapes.append(mpatches.Circle((x,y), r))
	elif vals[0] == 'line':
		x1, y1, x2, y2 = float(vals[1]), float(vals[2]), float(vals[3]), float(vals[4])
		shapes.append(mlines.Line2D((x1,x2), (y1,y2), lw=1))
	elif vals[0] == 'segment':
		x1, y1, x2, y2, r = float(vals[1]), float(vals[2]), float(vals[3]), float(vals[4]), float(vals[5])
		shapes.append(mlines.Line2D((x1,x2), (y1,y2), lw=r))
	elif vals[0] == 'rectangle':
		x1, y1, x2, y2 = float(vals[1]), float(vals[2]), float(vals[3]), float(vals[4])
		w, h = x2-x1, y2-t1
		shapes.append(mpatches.Rectangle((x1,y1), w, h))

def addShapes(ax, shapes):
	for s in shapes:
		if type(s) == mlines.Line2D:
			ax.add_line(s)
		else:
			ax.add_patch(s)

fig, ax = plt.subplots()

def init():
	plt.axis('equal')

def animate(dt):
	ax.clear()
	ax.set_xlim((0,600))
	ax.set_ylim((0,600))
	addShapes(ax, frames[dt])

anim = animation.FuncAnimation(fig, animate, init_func=init,
                               frames=len(frames), interval=20, blit=False)

# anim.to_html5_video()

plt.show()