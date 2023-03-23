#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <plotter.h>
#include <vector.h>
#include <aircraft.h>
#include <field.h>

int main()
{
	std::string filepath = STOCK_PATH;
	Plotter plt(filepath);
	OurVector<3> a;
	a.setValue(1);
	a[2] = 0.0001f;
	Aircraft aircraft(a);
	aircraft.calculateNewSpeed();
	Field obj(aircraft);
	obj.startMovement();

	OurVector<3> cord;
	for (int i = 0; i < 1000; ++i)
	{
		cord = obj.getCoordinate(i);
		plt.addPoint(cord[0], cord[1], cord[2]);
	}
	
	return 0;
}