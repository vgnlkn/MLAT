#include <main_class.h>
#include <plotter.h>
#include <vector.h>
#include <field.h>
#include <json_reader.h>
#include <plotter_creator.h>

MainClass::MainClass()
{
    _json_reader = new JSONReader(STOCK_PATH);

    _plt_creator = new PlotterCreator;
    _plt_mlat = _plt_creator->FactoryMethod(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_MLAT"));
    _plt_real = _plt_creator->FactoryMethod(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_REAL"));
    _plt_speed_real = _plt_creator->FactoryMethod(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_SPEED_REAL"));
    _plt_acceleration_real = _plt_creator->FactoryMethod(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_ACCELERATION_REAL"));

    _plt_filter = _plt_creator->FactoryMethod(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_FILTER"));
    _plt_speed_filter = _plt_creator->FactoryMethod(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_SPEED_FILTER"));
    _plt_acceleration_filter = _plt_creator->FactoryMethod(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_ACCELERATION_FILTER"));

    _plt_speed_standard_filter = _plt_creator->FactoryMethod(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_SPEED_STANDARD_FILTER"));
    _plt_standard_filter = _plt_creator->FactoryMethod(PLOTTER_PATH + _json_reader->getContent("STOCK_PATH_STANDARD_FILTER"));
    _plt_acceleration_standard_filter = _plt_creator->FactoryMethod(PLOTTER_PATH +
                                                    _json_reader->getContent("STOCK_PATH_ACCELERATION_STANDARD_FILTER"));

    _aircraft = new Aircraft;
    _field = new Field;
}

MainClass::~MainClass()
{
    delete _plt_creator;
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

    _field->setPlotterMLAT(dynamic_cast<Plotter*>(_plt_mlat));

    _field->setPlotterFlight(dynamic_cast<Plotter*>(_plt_real));
    _field->setPlotterSpeed(dynamic_cast<Plotter*>(_plt_speed_real));
    _field->setPlotterAcceleration(dynamic_cast<Plotter*>(_plt_acceleration_real));

    _field->setPlotterFilter(dynamic_cast<Plotter*>(_plt_filter));
    _field->setPlotterFilterSpeed(dynamic_cast<Plotter*>(_plt_speed_filter));
    _field->setPlotterFilterAcceleration(dynamic_cast<Plotter*>(_plt_acceleration_filter));

    _field->setPlotterStandardFilter(dynamic_cast<Plotter*>(_plt_standard_filter));
    _field->setPlotterFilterStandardSpeed(dynamic_cast<Plotter*>(_plt_speed_standard_filter));
    _field->setPlotterFilterStandardAcceleration(dynamic_cast<Plotter*>(_plt_acceleration_standard_filter));
}

void MainClass::runSimulation()
{
    setParameters();
    _field->startMovement();
}
