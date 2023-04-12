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

    ax.set_xlabel('x, km')
    ax.set_ylabel('y, km')
    ax.set_zlabel('z, km')
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


def draw2D(arr1: list, arr2: list, title: str) -> None:
    plt.plot(arr1, label='MLAT')
    plt.plot(arr2, label='Real')

    plt.title(title)

    plt.ylabel('km')
    plt.xlabel('iteration number')

    # Display the legend
    plt.legend()

    # Show the plot
    plt.show()


def count_diff(arr1: list, arr2: list) -> list:
    diff = list()

    for i in range(min(len(arr1), len(arr2))):
        diff.append(abs(arr1[i] - arr2[i]))

    return diff


def draw_diff(arr1: list, arr2: list, title: str) -> None:
    diff = count_diff(arr1, arr2)
    iterations = [int(i) for i in range(len(diff))]

    plt.figure()
    plt.title(title)
    plt.scatter(iterations, diff, label='modulus of difference', s=2)

    plt.ylabel('km')
    plt.xlabel('iteration number')
    plt.legend()
    plt.show()


draw2D(arr_x1, arr_x2, 'Coordinates of the aircraft on the axis x')
draw_diff(arr_x1, arr_x2, 'Modulus of difference on the axis x')

draw2D(arr_y1, arr_y2, 'Coordinates of the aircraft on the axis y')
draw_diff(arr_y1, arr_y2, 'Modulus of difference on the axis y')

draw2D(arr_z1, arr_z2, 'Coordinates of the aircraft on the axis z')
draw_diff(arr_z1, arr_z2, 'Modulus of difference on the axis z')
