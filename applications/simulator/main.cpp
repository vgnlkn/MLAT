#include <iostream>
#include <plotter.h>
#include <vector.h>
#include <aircraft.h>
#include <field.h>

int main()
{
    const std::string filepath_mlat = STOCK_PATH_MLAT;
    const std::string filepath_real = STOCK_PATH_REAL;
    const std::string filepath_filter = STOCK_PATH_FILTER;

	Plotter plt_mlat(filepath_mlat);
	Plotter plt_real(filepath_real);
	Plotter plt_filter(filepath_filter);

	OurVector<3> a;
	a.setValue(1);
	a[2] = 0.001f;

	Aircraft aircraft(a);
	aircraft.calculateNewSpeed();

	Field obj(aircraft);

	obj.setPlotterMLAT(&plt_mlat);
	obj.setPlotterFlight(&plt_real);
    obj.setPlotterFilter(&plt_filter);

	obj.startMovement();

	
	return 0;
}