import matplotlib.pyplot as plt
import matplotlib.animation as animation
import sys

fig = plt.figure()
ax = plt.axes(projection='3d')


def read(a, b, c, d, e, f, data1, data2):
    for line1, line2 in zip(data1, data2):
        if line1 == '' or line2 == '':
            break
        x1, y1, z1 = line1.split()
        x2, y2, z2 = line2.split()
        a.append(float(x1))
        b.append(float(y1))
        c.append(float(z1))

        d.append(float(x2))
        e.append(float(y2))
        f.append(float(z2))


def animate(i):
    data1 = open(sys.argv[1], 'r').read()
    lines1 = data1.split('\n')

    data2 = open(sys.argv[2], 'r').read()
    lines2 = data2.split('\n')

    xs1, ys1, zs1 = [], [], []
    xs2, ys2, zs2 = [], [], []
    read(xs1, ys1, zs1, xs2, ys2, zs2, lines1, lines2)

    ax.clear()
    ax.plot(xs1, ys1, zs1, label='Inverse problem')
    ax.plot(xs2, ys2, zs2, label='Direct problem')

    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    ax.set_title('MLAT')
    ax.legend()


ani = animation.FuncAnimation(fig, animate, interval=10)
plt.show()

data = open(sys.argv[1], 'r').read()
lines = data.split('\n')

data2 = open(sys.argv[2], 'r').read()
lines2 = data2.split('\n')

arr_x1, arr_y1, arr_z1 = [], [], []
arr_x2, arr_y2, arr_z2 = [], [], []
read(arr_x1, arr_y1, arr_z1, arr_x2, arr_y2, arr_z2, lines, lines2)

# Create a line plot for xoy
plt.plot(arr_x1, label='MLAT')
plt.plot(arr_x2, label='Real')

# Add labels and a title
plt.title('Coordinates of the aircraft on the Ox')

# Display the legend
plt.legend()

# Show the plot
plt.show()

# Create a line plot for Oy
plt.plot(arr_y1, label='MLAT')
plt.plot(arr_y2, label='Real')

plt.title('Coordinates of the aircraft on the Oy')

# Display the legend
plt.legend()

# Show the plot
plt.show()

# Create a line plot for Oz
plt.plot(arr_z1, label='MLAT')
plt.plot(arr_z2, label='Real')

plt.title('Coordinates of the aircraft on the Oz')

# Display the legend
plt.legend()

# Show the plot
plt.show()
