#include <field.h>
#include <aircraft.h>
#include <chrono>
#include <thread>


Field::Field() :
        _towers(new Tower[TOWERS_COUNT]),
        _plt_mlat(nullptr), _plt_flight(nullptr),
        _tower_count(TOWERS_COUNT)
{
    _current_position[2] = 10;
}

void Field::startMovement()
{
    initialize();

    for (int i = 0;; ++i)
    {
        updateAircraftPosition();
        checkHeight();
        updateAircraftSpeed();
        sendSignalsToTowers();
        processSignals();
        updatePlot();
    }
}

void Field::initialize()
{
    setTowers();
    setAircraftTowers();
    _aircraft.checkAcceleration();
}

void Field::updateAircraftPosition()
{
    _current_position = _current_position +
                        _aircraft.getSpeed() * (1.f / kilometer);
}

void Field::updateAircraftSpeed()
{
    _aircraft.calculateNewSpeed();
    _aircraft.checkSpeed();
}

void Field::sendSignalsToTowers()
{
    float toa;
    for (uint16_t j = 0; j < _tower_count; ++j)
    {
        Tower& tower = _towers[j];
        toa = _aircraft.sendSignal(tower, _current_position);
        _processor.addTOA(tower.getID(), toa);
        _processor.setTower(tower.getID(), tower);
    }
}

void Field::updatePlot()
{
    if (_plt_flight)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        _plt_flight->addPoint(_current_position[0], _current_position[1],
                              _current_position[2]);
    }
}

void Field::checkHeight()
{
    if (_current_position[2] > 11.5f && _aircraft.getSpeed()[2] > 0.f)
    {
        decreaseVerticalSpeed();
    }
    if (_aircraft.getSpeed()[2] < -1.f)
    {
        stopVerticalSpeed();
    }
}

void Field::decreaseVerticalSpeed()
{
    OurVector<3> new_speed = _aircraft.getSpeed();
    new_speed[2] /= 16;

    OurVector<3> new_acceleration = _aircraft.getAcceleration();
    new_acceleration[2] = new_acceleration[2] < 0.f ? new_acceleration[2] : -new_acceleration[2];

    _aircraft.setSpeed(new_speed);
    _aircraft.setAcceleration(new_acceleration);
}

void Field::stopVerticalSpeed()
{
    OurVector<3> new_speed = _aircraft.getSpeed();
    new_speed[2] = 0;

    OurVector<3> new_acceleration = _aircraft.getAcceleration();
    new_acceleration[2] = 0;

    _aircraft.setSpeed(new_speed);
    _aircraft.setAcceleration(new_acceleration);
}

void Field::setTowers()
{
    OurVector<3> tower_position;
    tower_position[0] = -1500;
    tower_position[1] = 400;
    tower_position[2] = 50;

    _towers[0].setID(0);
    _towers[0].setPosition(tower_position);

    tower_position[0] = 2000;
    tower_position[1] = 3000;
    tower_position[2] = 1000;

    _towers[1].setID(1);
    _towers[1].setPosition(tower_position);

    tower_position[0] = 5000;
    tower_position[1] = 12000;
    tower_position[2] = 100;

    _towers[2].setID(2);
    _towers[2].setPosition(tower_position);

    tower_position[0] = 11000;
    tower_position[1] = -4000;
    tower_position[2] = 250;

    _towers[3].setID(3);
    _towers[3].setPosition(tower_position);
}

void Field::setAircraftTowers()
{
    for (uint16_t i = 0; i < _tower_count; ++i)
    {
        _aircraft.setNewTower(_towers[i]);
    }
}
