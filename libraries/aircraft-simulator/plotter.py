import matplotlib.pyplot as plt
import matplotlib.animation as animation
import sys

fig = plt.figure()
ax = plt.axes(projection='3d')


def read(data1, data2):
    xs1, ys1, zs1 = [], [], []
    xs2, ys2, zs2 = [], [], []
    for line1, line2 in zip(data1, data2):
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

    xs3 = [x2 + 50 for x2 in xs2]
    ys3 = [y2 + 50 for y2 in ys2]
    zs3 = [z2 + 1 for z2 in zs2]

    return xs1, ys1, zs1, xs2, ys2, zs2, xs3, ys3, zs3


def animate(i):
    data1 = open(sys.argv[1], 'r').read().split('\n')
    data2 = open(sys.argv[2], 'r').read().split('\n')

    xs1, ys1, zs1, xs2, ys2, zs2, xs3, ys3, zs3 = read(data1, data2)

    ax.clear()
    ax.plot(xs1, ys1, zs1, label='MLAT')
    ax.plot(xs2, ys2, zs2, label='Real values')
    ax.plot(xs3, ys3, zs3, label='Inverse problem')

    ax.set_xlabel('x, km')
    ax.set_ylabel('y, km')
    ax.set_zlabel('z, km')
    ax.set_title('MLAT')
    ax.legend()


ani = animation.FuncAnimation(fig, animate, interval=10)
plt.show()

data = open(sys.argv[1], 'r').read().split('\n')
data2 = open(sys.argv[2], 'r').read().split('\n')

arr_x1, arr_y1, arr_z1, arr_x2, arr_y2, arr_z2, arr_x3, arr_y3, arr_z3 = read(data, data2)


def draw2D(arr1, arr2, arr3, title):
    plt.plot(arr1, label='MLAT')
    plt.plot(arr2, label='Real')
    plt.plot(arr3, label='Inverse problem')

    plt.title(title)

    plt.ylabel('km')
    plt.xlabel('iteration number')

    # Display the legend
    plt.legend()

    # Show the plot
    plt.show()


def count_diff(arr1, arr2):
    diff = []

    for i in range(min(len(arr1), len(arr2))):
        diff.append(abs(arr1[i] - arr2[i]))

    return diff


def draw_diff(arr1, arr2, title):
    diff = count_diff(arr1, arr2)
    iterations = [int(i) for i in range(len(diff))]

    plt.figure()
    plt.title(title)
    plt.scatter(iterations, diff, label='modulus of difference')

    plt.ylabel('km')
    plt.xlabel('iteration number')
    plt.legend()
    plt.show()


draw2D(arr_x1, arr_x2, arr_x3, 'Coordinates of the aircraft on the axis x')
draw_diff(arr_x1, arr_x2, 'Modulus of difference on the axis x')

draw2D(arr_y1, arr_y2, arr_y3, 'Coordinates of the aircraft on the axis y')
draw_diff(arr_y1, arr_y2, 'Modulus of difference on the axis y')

draw2D(arr_z1, arr_z2, arr_z3, 'Coordinates of the aircraft on the axis z')
draw_diff(arr_z1, arr_z2, 'Modulus of difference on the axis z')
