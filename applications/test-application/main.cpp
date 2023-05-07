#include <matrix.h>
#include <vector.h>
#include <iostream>
#include <random>
#include <cmath>
#include <motion_filter.h>


class Motion
{
public:
	Motion(double x0, double v0, double a0):
		x(x0), v(v0), a(a0)
	{

	}

	void update(double time_delta)
	{
		x += v * time_delta + a * time_delta * time_delta * 0.5;
		v += a * time_delta;
	}

public:
	double x, v, a;
};

int main()
{
    std::vector<double> real_values, noise_values;

    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> d{0, 1e-5};
	std::vector<double> x, v, a;

	const int iterarions = 1e5;
	double time_delta = 1e-4;
	Motion motion(0, 0, 10);
	MotionFilter mfilter;
	OurVector<1> observation;
	OurVector<3> state;
	state[0] = motion.x;
	state[1] = motion.v;
	state[2] = motion.a;
	//mfilter.setInitial(state);
	//motion.update(time_delta);
	for (int i = 0; i < iterarions; ++i)
	{

        real_values.push_back(motion.x);
        motion.x += d(gen);
		motion.update(time_delta);

        noise_values.push_back(motion.x);

        observation[0] = motion.x;

		state = mfilter.filter(observation);
		x.push_back(state[0]);
		v.push_back(state[1]);
		a.push_back(state[2]);
	}

//	std::cout << (*(v.end() - 1) / 3)* (*(v.end() - 1) / 3) << std::endl;
	double* xp = x.data();
	double* xr = real_values.data();
	double* vp = v.data();
	double* ap = a.data();

	for (int i = 0; i < real_values.size(); ++i)
	{
		std::cout << std::abs(real_values[i] - x[i]) << std::endl;
	}

	return 0;
}