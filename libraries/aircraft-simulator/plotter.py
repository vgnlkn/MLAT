import matplotlib.pyplot as plt
import matplotlib.animation as animation
import sys

fig = plt.figure()
ax = plt.axes(projection='3d')


def read(data_mlat: list, data_real: list, data_filter: list):
    xs1, ys1, zs1 = [], [], []
    xs2, ys2, zs2 = [], [], []
    xs3, ys3, zs3 = [], [], []
    for line1, line2, line3 in zip(data_mlat, data_real, data_filter):
        if line1 == '' or line2 == '' or line3 == '':
            break
        x1, y1, z1 = line1.split()
        x2, y2, z2 = line2.split()
        x3, y3, z3 = line3.split()

        xs1.append(float(x1))
        ys1.append(float(y1))
        zs1.append(float(z1))

        xs2.append(float(x2))
        ys2.append(float(y2))
        zs2.append(float(z2))

        xs3.append(float(x3))
        ys3.append(float(y3))
        zs3.append(float(z3))

    return xs1, ys1, zs1, xs2, ys2, zs2, xs3, ys3, zs3


def read_speed(data_filter_speed: list, data_real_speed: list):
    xs1, ys1, zs1 = [], [], []
    xs2, ys2, zs2 = [], [], []
    for line1, line2 in zip(data_filter_speed, data_real_speed):
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

    return xs1, ys1, zs1, xs2, ys2, zs2


def animate(i):
    data_mlat = open(sys.argv[1], 'r').read().split('\n')
    data_real = open(sys.argv[2], 'r').read().split('\n')
    data_filter = open(sys.argv[3], 'r').read().split('\n')

    xs1, ys1, zs1, xs2, ys2, zs2, xs3, ys3, zs3 = read(data_mlat, data_real, data_filter)

    ax.clear()
    ax.plot(xs1, ys1, zs1, label='MLAT', color='blue')
    ax.plot(xs2, ys2, zs2, label='Real values', color='red')
    ax.plot(xs3, ys3, zs3, label='Inverse problem', color='black')

    ax.set_xlabel('x, km')
    ax.set_ylabel('y, km')
    ax.set_zlabel('z, km')
    ax.set_title('MLAT')
    ax.legend()


ani = animation.FuncAnimation(fig, animate, interval=10)
plt.show()

data_mlat = open(sys.argv[1], 'r').read().split('\n')
data_real = open(sys.argv[2], 'r').read().split('\n')
data_filter = open(sys.argv[3], 'r').read().split('\n')
data_filter_speed = open(sys.argv[4], 'r').read().split('\n')
data_real_speed = open(sys.argv[5], 'r').read().split('\n')

arr_x_mlat, arr_y_mlat, arr_z_mlat, arr_x_real, arr_y_real, \
    arr_z_real, arr_x_filter, arr_y_filter, arr_z_filter = read(data_mlat, data_real, data_filter)

x_filter_speed, y_filter_speed, z_filter_speed, x_real_speed, y_real_speed, z_real_speed \
    = read_speed(data_filter_speed, data_real_speed)


def draw2D(arr_mlat: list, arr_real: list, arr_filter: list, title: str):
    plt.plot(arr_mlat, label='MLAT')
    plt.plot(arr_real, label='Real')
    plt.plot(arr_filter, label='Inverse problem')

    plt.title(title)

    plt.ylabel('km')
    plt.xlabel('iteration number')

    # Display the legend
    plt.legend()

    # Show the plot
    plt.show()


def count_diff(first_arr: list, second_arr: list) -> list:
    diff = []

    for i in range(min(len(first_arr), len(second_arr))):
        diff.append(abs(first_arr[i] - second_arr[i]))

    return diff


def draw_diff(first_arr: list, second_arr: list, title: str):
    diff = count_diff(first_arr, second_arr)
    iterations = [int(i) for i in range(len(diff))]

    plt.figure()
    plt.title(title)
    plt.scatter(iterations, diff, label='modulus of difference', s=2)

    plt.ylabel('km')
    plt.xlabel('iteration number')
    plt.legend()
    plt.show()


fig, axs = plt.subplots(3, 1, figsize=(8, 10))

# Plot for x
axs[0].plot(x_real_speed, label='Real')
axs[0].plot(x_filter_speed, label='Filter')
axs[0].set_title('x speed')
axs[0].set_ylabel('m/s')
axs[0].legend()

# Plot for y
axs[1].plot(y_real_speed, label='Real')
axs[1].plot(y_filter_speed, label='Filter')
axs[1].set_title('y speed')
axs[1].set_ylabel('m/s')
axs[1].legend()

# Plot for z
axs[2].plot(z_real_speed, label='Real')
axs[2].plot(z_filter_speed, label='Filter')
axs[2].set_title('z speed')
axs[2].set_ylabel('m/s')
axs[2].set_xlabel('iteration number')
axs[2].legend()

plt.show()

'''
draw2D(arr_x_mlat, arr_x_real, arr_x_filter, 'Coordinates of the aircraft on the axis x')
draw_diff(arr_x_mlat, arr_x_real, 'Modulus of difference on the axis x')

draw2D(arr_y_mlat, arr_y_real, arr_y_filter, 'Coordinates of the aircraft on the axis y')
draw_diff(arr_y_mlat, arr_y_real, 'Modulus of difference on the axis y')

draw2D(arr_z_mlat, arr_z_real, arr_z_filter, 'Coordinates of the aircraft on the axis z')
draw_diff(arr_z_mlat, arr_z_real, 'Modulus of difference on the axis z')
'''
