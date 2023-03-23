#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <plotter.h>

#define RAND_ (float)(rand()) / RAND_MAX

int main()
{
	std::string filepath = STOCK_PATH;
	Plotter plt(filepath);
	char c;
	for (int i = 0; i < 100; ++i)
	{
		float x = RAND_;
		float y = RAND_;
		float z = RAND_;
		std::cin >> c;
		plt.addPoint(x, y, z);
		
	}
	
	return 0;
}