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
//	OurMatrix<4, 3, double> a;
//	float k = 1;
//	for (int i = 0; i < 4; ++i)
//	{
//		for (int j = 0; j < 3; ++j)
//		{
//			a[i][j] = k++;
//		}
//	}
//
//	//std::cout << a << std::endl;
//
//	auto qr = a.QRDecomposition();
//	auto Q = qr.first;
//	auto R = qr.second;
//
//
//	auto b = a.pseudoInverse();
//	std::cout << "Q:\n" << Q << "\n\nR:\n" << R << "\n\n\n\nQR=\n" << Q * R << "\n\n\n";
//
//	std::cout << b << "\n\n\n";
//
//	std::cout << a * b * a << "\n\n\n";

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