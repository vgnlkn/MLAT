#include <matrix.h>
#include <vector.h>
#include <iostream>
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
	std::vector<double> x, v, a;
	const int iterarions = 100;
	double time_delta = 0.5;
	Motion motion(0, 0, 1);
	MotionFilter mfilter;
	OurVector<3> state, filtered;

	for (int i = 0; i < iterarions; ++i)
	{
		motion.update(time_delta);
		state[0] = motion.x;
		state[1] = motion.v;
		state[2] = motion.a;
		filtered = mfilter.filter(state);

		/*x.push_back(motion.x);
		v.push_back(motion.v);
		a.push_back(motion.a);*/

		x.push_back(filtered[0]);
		v.push_back(filtered[1]);
		a.push_back(filtered[2]);
	}

	double* xp = x.data();
	double* vp = v.data();
	double* ap = a.data();





	return 0;
}