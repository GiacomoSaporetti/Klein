import matplotlib.pyplot as plt
import numpy as np
points = []
f = open('plot.txt', 'r')

#print(f.read())

for line in f:
    points.append(np.fromstring(line, dtype=int, sep=','))

mat = np.matrix(points)

mat = mat.transpose()

mat = mat.tolist()
xpoints = mat[0]

ypoints = mat[1]
plt.plot(xpoints ,ypoints)
ypoints = mat[2]
plt.plot(xpoints ,ypoints)
ypoints = mat[3]
plt.plot(xpoints ,ypoints)
plt.show()


