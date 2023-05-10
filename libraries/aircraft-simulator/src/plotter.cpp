#include <cassert>
#include <iostream>
#include <plotter.h>

void Plotter::addPoint(double x, double y, double z)
{
	assert(_file->is_open());
	(*_file) << x << ' ' << y << ' ' << z << std::endl;
}
