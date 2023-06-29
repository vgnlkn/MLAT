#include <main_class.h>
#include <plotter.h>
#include <vector.h>
#include <field.h>
#include <json_reader.h>

MainClass::MainClass()
{
    _json_reader = new JSONReader(STOCK_PATH);

    _plt_mlat = new Plotter(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_MLAT"));

    _plt_real = new Plotter(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_REAL"));
    _plt_speed_real = new Plotter(PLOTTER_PATH +_json_reader->getContent("STOCK_PATH_SPEED_REAL"));
    _plt_acceleration_real = new Plotter(PLOTTER_PATH +_json_reader->getContent("STOCK_PATH_ACCELERATION_REAL"));

    _plt_filter = new Plotter(PLOTTER_PATH +_json_reader->getContent("STOCK_PATH_FILTER"));
    _plt_speed_filter = new Plotter(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_SPEED_FILTER"));
    _plt_acceleration_filter = new Plotter(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_ACCELERATION_FILTER"));

    _plt_speed_standard_filter = new Plotter(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_SPEED_STANDARD_FILTER"));
    _plt_standard_filter = new Plotter(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_STANDARD_FILTER"));
    _plt_acceleration_standard_filter = new Plotter(PLOTTER_PATH +
                                                    _json_reader->getContent("STOCK_PATH_ACCELERATION_STANDARD_FILTER"));

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
    delete _plt_acceleration_standard_filter;
    delete _plt_speed_standard_filter;
    delete _plt_standard_filter;
    delete _aircraft;
    delete _field;
}

void MainClass::setParameters()
{
    OurVector<3> acceleration;
    acceleration.setValue(1e-4);
    acceleration[1] = 5e-5;
    acceleration[2] = 3e-6;
    _aircraft->setAcceleration(acceleration);

    _field->setAircraft(*_aircraft);

    _field->setPlotterMLAT(_plt_mlat);

    _field->setPlotterFlight(_plt_real);
    _field->setPlotterSpeed(_plt_speed_real);
    _field->setPlotterAcceleration(_plt_acceleration_real);

    _field->setPlotterFilter(_plt_filter);
    _field->setPlotterFilterSpeed(_plt_speed_filter);
    _field->setPlotterFilterAcceleration(_plt_acceleration_filter);

    _field->setPlotterStandardFilter(_plt_standard_filter);
    _field->setPlotterFilterStandardSpeed(_plt_speed_standard_filter);
    _field->setPlotterFilterStandardAcceleration(_plt_acceleration_standard_filter);
}

void MainClass::runSimulation()
{
    setParameters();
    _field->startMovement();
}
