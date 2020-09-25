import matplotlib.pyplot as plt
from sys import stdin

colors_map = ['#ff0000', '#00ff00', '#0000ff', '#f0000f', '#ff00ff', '#000000']

x = list()
y = list()
colors = list()

for line in stdin:
    args = [float(i) for i in line.strip().split(';')]
    x.append(args[0])
    y.append(args[1])
    colors.append(colors_map[int(args[2]) % len(colors_map)])

plt.scatter(x, y, c=colors)
plt.show()