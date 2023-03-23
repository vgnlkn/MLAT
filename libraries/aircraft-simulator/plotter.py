import matplotlib.pyplot as plt
import matplotlib.animation as animation
import sys


fig = plt.figure()
ax = plt.axes(projection='3d')


def animate(i):
    data = open(sys.argv[1], 'r').read()
    lines = data.split('\n')
    xs, ys, zs = [], [], []
    
    for line in lines:
        if line == '':
            break
        x, y, z = line.split() 
        xs.append(float(x))
        ys.append(float(y))
        zs.append(float(z))

    ax.clear()
    ax.plot(xs, ys, zs)

    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('MLAT')


ani = animation.FuncAnimation(fig, animate, interval=10)
plt.show()
