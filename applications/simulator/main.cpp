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
	a[2] = 0.001f;
	Aircraft aircraft(a);
	aircraft.calculateNewSpeed();
	Field obj(aircraft);
	obj.setPlotter(&plt);
	obj.startMovement();


	
	return 0;
}