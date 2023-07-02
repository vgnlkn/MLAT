#ifndef MLAT_PLOTTER_H
#define MLAT_PLOTTER_H

#include <fstream>
#include <string>
#include <interface_product.h>

/*! \class Plotter
*   \brief Converts data to file.
* 	It translates the data needed to build a graph into some file.
* 	Then the script that launches the chart is manually launched.
*/
class Plotter final : public IProduct
{
public:
	//! Constructor
	inline explicit Plotter(const std::string& filepath) : _file(new std::ofstream(filepath)) {}
	//! Destructor
	inline ~Plotter() final { delete _file; }
    //! Insert point to the file
	void addPoint(double x, double y, double z) final;
	
private:
    //! Result file with points
	std::ofstream* _file;
};
#endif