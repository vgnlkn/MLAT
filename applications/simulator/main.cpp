#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <plotter.h>
#include <vector.h>
#include <aircraft.h>
#include <field.h>

#define RAND_ (float)(rand()) / RAND_MAX

int main()
{
	std::string filepath = STOCK_PATH;
	Plotter plt(filepath);
	/*char c;
	for (int i = 0; i < 100; ++i)
	{
		float x = RAND_;
		float y = RAND_;
		float z = RAND_;
		std::cin >> c;
		plt.addPoint(x, y, z);
	}*/

	OurVector<3> a;
	a.setValue(1);

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