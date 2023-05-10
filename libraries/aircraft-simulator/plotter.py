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
data_filter_acceleration = open(sys.argv[6], 'r').read().split('\n')
data_real_acceleration = open(sys.argv[7], 'r').read().split('\n')

arr_x_mlat, arr_y_mlat, arr_z_mlat, arr_x_real, arr_y_real, \
    arr_z_real, arr_x_filter, arr_y_filter, arr_z_filter = read(data_mlat, data_real, data_filter)

x_filter_speed, y_filter_speed, z_filter_speed, x_real_speed, y_real_speed, z_real_speed \
    = read_speed(data_filter_speed, data_real_speed)

x_filter_acceleration, y_filter_acceleration, z_filter_acceleration,\
    x_real_acceleration, y_real_acceleration, z_real_acceleration \
    = read_speed(data_filter_acceleration, data_real_acceleration)


fig, axs = plt.subplots(3, 3, figsize=(8, 10))

# Coordinates

# Plot for x coordinate
axs[0][0].plot(arr_x_real, label='Real')
axs[0][0].plot(arr_x_filter, label='Filter')
axs[0][0].set_title('Coordinates of the aircraft on the axis x')
axs[0][0].set_ylabel('km')
axs[0][0].legend()

# Plot for y coordinate
axs[1][0].plot(arr_y_real, label='Real')
axs[1][0].plot(arr_y_filter, label='Filter')
axs[1][0].set_title('Coordinates of the aircraft on the axis y')
axs[1][0].set_ylabel('km')
axs[1][0].legend()

# Plot for z coordinate
axs[2][0].plot(arr_z_real, label='Real')
axs[2][0].plot(arr_z_filter, label='Filter')
axs[2][0].set_title('Coordinates of the aircraft on the axis z')
axs[2][0].set_ylabel('km')
axs[2][0].set_xlabel('iteration number')
axs[2][0].legend()

# Speed

# Plot for x speed
axs[0][1].plot(x_real_speed, label='Real')
axs[0][1].plot(x_filter_speed, label='Filter')
axs[0][1].set_title('x speed')
axs[0][1].set_ylabel('m/s')
axs[0][1].legend()

# Plot for y speed
axs[1][1].plot(y_real_speed, label='Real')
axs[1][1].plot(y_filter_speed, label='Filter')
axs[1][1].set_title('y speed')
axs[1][1].set_ylabel('m/s')
axs[1][1].legend()

# Plot for z speed
axs[2][1].plot(z_real_speed, label='Real')
axs[2][1].plot(z_filter_speed, label='Filter')
axs[2][1].set_title('z speed')
axs[2][1].set_ylabel('m/s')
axs[2][1].set_xlabel('iteration number')
axs[2][1].legend()

# Acceleration

# Plot for x speed
axs[0][2].plot(x_real_acceleration, label='Real')
axs[0][2].plot(x_filter_acceleration, label='Filter')
axs[0][2].set_title('x acceleration')
axs[0][2].set_ylabel('m/s^2')
axs[0][2].legend()

# Plot for y speed
axs[1][2].plot(y_real_acceleration, label='Real')
axs[1][2].plot(y_filter_acceleration, label='Filter')
axs[1][2].set_title('y acceleration')
axs[1][2].set_ylabel('m/s^2')
axs[1][2].legend()

# Plot for z speed
axs[2][2].plot(z_real_acceleration, label='Real')
axs[2][2].plot(z_filter_acceleration, label='Filter')
axs[2][2].set_title('z acceleration')
axs[2][2].set_ylabel('m/s^2')
axs[2][2].set_xlabel('iteration number')
axs[2][2].legend()

plt.show()
