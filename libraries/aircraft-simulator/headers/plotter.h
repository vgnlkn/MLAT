#ifndef MLAT_PLOTTER_H
#define MLAT_PLOTTER_H

#include <fstream>
#include <string>

class Plotter
{
public:
	inline Plotter(const std::string& filepath) : _file(new std::ofstream(filepath)) {}
	inline ~Plotter() { if (_file) { delete _file; } }
	void addPoint(float x, float y, float z);
private:
	std::ofstream* _file;

};
#endif