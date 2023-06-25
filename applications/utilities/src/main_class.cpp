#include <main_class.h>
#include <plotter.h>
#include <vector.h>
#include <field.h>

MainClass::MainClass()
{
    _plt_mlat = new Plotter(STOCK_PATH_MLAT);
    _plt_real = new Plotter(STOCK_PATH_REAL);
    _plt_filter = new Plotter(STOCK_PATH_FILTER);
    _plt_speed_filter = new Plotter(STOCK_PATH_SPEED_FILTER);
    _plt_speed_real = new Plotter(STOCK_PATH_SPEED_REAL);
    _plt_acceleration_filter = new Plotter(STOCK_PATH_ACCELERATION_FILTER);
    _plt_acceleration_real = new Plotter(STOCK_PATH_ACCELERATION_REAL);
    _aircraft = new Aircraft;
    _field = new Field;
}

MainClass::~MainClass()
{
    delete _plt_mlat;
    delete _plt_real;
    delete _plt_filter;
    delete _plt_speed_filter;
    delete _plt_speed_real;
    delete _plt_acceleration_filter;
    delete _plt_acceleration_real;
    delete _aircraft;
    delete _field;
}

void MainClass::setParameters()
{
    OurVector<3> acceleration;
    acceleration.setValue(1e-4);
    acceleration[2] = 2e-6;
    _aircraft->setAcceleration(acceleration);

    _field->setAircraft(*_aircraft);
    _field->setPlotterMLAT(_plt_mlat);
    _field->setPlotterFlight(_plt_real);
    _field->setPlotterFilter(_plt_filter);
    _field->setPlotterFilterSpeed(_plt_speed_filter);
    _field->setPlotterSpeed(_plt_speed_real);
    _field->setPlotterAcceleration(_plt_acceleration_real);
    _field->setPlotterFilterAcceleration(_plt_acceleration_filter);
}

void MainClass::runSimulation()
{
    setParameters();
    _field->startMovement();
}
