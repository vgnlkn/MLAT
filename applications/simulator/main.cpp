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
    const std::string filepath_speed_filter = STOCK_PATH_SPEED_FILTER;
    const std::string filepath_real_speed = STOCK_PATH_SPEED_REAL;
    const std::string filepath_filter_acceleration = STOCK_PATH_ACCELERATION_FILTER;
    const std::string filepath_real_acceleration = STOCK_PATH_ACCELERATION_REAL;

	Plotter plt_mlat(filepath_mlat);
	Plotter plt_real(filepath_real);
	Plotter plt_filter(filepath_filter);
	Plotter plt_speed_filter(filepath_speed_filter);
	Plotter plt_speed_real(filepath_real_speed);
	Plotter plt_acceleration_filter(filepath_filter_acceleration);
	Plotter plt_acceleration_real(filepath_real_acceleration);

	OurVector<3> acceleration;
	acceleration.setValue(1);
    acceleration[2] = 1.f;

	Aircraft aircraft(acceleration);
	aircraft.calculateNewSpeed();

	Field obj(aircraft);

	obj.setPlotterMLAT(&plt_mlat);
	obj.setPlotterFlight(&plt_real);
    obj.setPlotterFilter(&plt_filter);
    obj.setPlotterFilterSpeed(&plt_speed_filter);
    obj.setPlotterSpeed(&plt_speed_real);
    obj.setPlotterAcceleration(&plt_acceleration_real);
    obj.setPlotterFilterAcceleration(&plt_acceleration_filter);

	obj.startMovement();

	
	return 0;
}