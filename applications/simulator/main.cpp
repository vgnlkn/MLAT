#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <plotter.h>
#include <vector.h>
#include <aircraft.h>
#include <field.h>
#include <matrix.h>

int main()
{
    const std::string filepath1 = STOCK_PATH1;
    const std::string filepath2 = STOCK_PATH2;
	Plotter plt1(filepath1);
	Plotter plt2(filepath2);

	OurVector<3> a;
	a.setValue(1);
	a[2] = 0.001f;

	Aircraft aircraft(a);
	aircraft.calculateNewSpeed();

	Field obj(aircraft);

	obj.setPlotterMLAT(&plt1);
	obj.setPlotterFlight(&plt2);

	obj.startMovement();

	
	return 0;
}