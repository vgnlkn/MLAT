#include <matrix.h>
#include <vector.h>
#include <iostream>
#include <random>
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
		v += a;
	}

public:
	double x, v, a;
};

int main()
{
    std::vector<Motion> real_values, noise_values;

    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> d{1e-4, 1e-5};
	std::vector<double> x, v, a;

	const int iterarions = 100;
	double time_delta = 0.5;
	Motion motion(0, 0, 1);
	MotionFilter mfilter;
	OurVector<1> observation;
	OurVector<3> state;
	for (int i = 0; i < iterarions; ++i)
	{
		motion.update(time_delta);
        real_values.push_back(motion);

        motion.a += d(gen);
        motion.v += d(gen);
        motion.x += d(gen);

        noise_values.push_back(motion);

		state = mfilter.filter(observation);
		x.push_back(state[0]);
		v.push_back(state[1]);
		a.push_back(state[2]);
	}

    for (auto i : noise_values)
        std::cout << i.x << " " << i.v << " " << i.a << "\n";

    for (auto j : real_values)
        std::cout << j.x << " " << j.v << " " << j.a << "\n";

	double* xp = x.data();
	double* vp = v.data();
	double* ap = a.data();


	return 0;
}