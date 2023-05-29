#include <cassert>
#include <iostream>
#include <plotter.h>

void Plotter::addPoint(long double x, long double y, long double z)
{
	assert(_file->is_open());
	(*_file) << x << ' ' << y << ' ' << z << std::endl;
}
