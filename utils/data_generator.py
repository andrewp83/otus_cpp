from random import randrange, uniform
from math import sqrt
from functools import partial


STAR_POINTS_COUNT = 200


def generate_circle_point(radius, offset = (0, 0)):
	sign = 1 if uniform(0, 1) >= 0.5 else -1

	x = 2 * radius * uniform(0, 1) - radius
	y = sign * sqrt(radius * radius - x * x)

	x += offset[0]
	y += offset[1]

	return x, y


def generate_points(count, generator):
	for _ in xrange(count):
		x, y = generator()
		print("{};{}".format(x, y))


generate_points(STAR_POINTS_COUNT, lambda : (randrange(-100, 101), randrange(-100,101)))

#generate_points(50, partial(generate_circle_point, 0.5))
#generate_points(50, partial(generate_circle_point, 10.0))
#generate_points(50, partial(generate_circle_point, 4.0, (25, 25)))