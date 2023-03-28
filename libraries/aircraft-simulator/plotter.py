import matplotlib.pyplot as plt
import matplotlib.animation as animation
import sys

fig = plt.figure()
ax = plt.axes(projection='3d')


def animate(i):
    data1 = open(sys.argv[1], 'r').read()
    lines1 = data1.split('\n')

    data2 = open(sys.argv[2], 'r').read()
    lines2 = data2.split('\n')

    xs1, ys1, zs1 = [], [], []
    xs2, ys2, zs2 = [], [], []
    for line1, line2 in zip(lines1, lines2):
        if line1 == '' or line2 == '':
            break
        x1, y1, z1 = line1.split()
        x2, y2, z2 = line2.split()
        xs1.append(float(x1))
        ys1.append(float(y1))
        zs1.append(float(z1))

        xs2.append(float(x2))
        ys2.append(float(y2))
        zs2.append(float(z2))

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

arr_x, arr_y, arr_z = [], [], []
for line in lines:
    if line == '':
        break
    x, y, z = line.split()
    arr_x.append(float(x))
    arr_y.append(float(y))
    arr_z.append(float(z))

# Create a line plot for xoy
plt.plot(arr_x, arr_y, label='xoy')

# Add labels and a title
plt.xlabel('x')
plt.ylabel('y')
plt.title('Coordinates of the aircraft on the xoy plane')

# Display the legend
plt.legend()

# Show the plot
plt.show()

# Create a line plot for xoz
plt.plot(arr_x, arr_z, label='xoz')

# Add labels and a title
plt.xlabel('x')
plt.ylabel('z')
plt.title('Coordinates of the aircraft on the xoz plane')

# Display the legend
plt.legend()

# Show the plot
plt.show()

# Create a line plot for yoz
plt.plot(arr_y, arr_z, label='yoz')

# Add labels and a title
plt.xlabel('y')
plt.ylabel('z')
plt.title('Coordinates of the aircraft on the yoz plane')

# Display the legend
plt.legend()

# Show the plot
plt.show()
