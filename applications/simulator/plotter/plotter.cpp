#include <cassert>
#include <iostream>
#include "plotter.h"

void Plotter::addPoint(float x, float y, float z)
{
	assert(_file->is_open());
	(*_file) << x << ' ' << y << ' ' << z << std::endl;
}
