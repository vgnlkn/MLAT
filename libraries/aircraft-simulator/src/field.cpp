#include <field.h>
#include <aircraft.h>
#include <chrono>
#include <thread>

Field::Field() : _coordinates(new OurVector<3>[1000]), _times(new float[1000])
{
    _current_position[2] = 10;
}

Field::~Field()
{
    delete[] _coordinates;
    delete[] _times;
}

void Field::startMovement()
{
    _aircraft.checkAcceleration();
    for(int i=0;;++i)
    {
        _current_position = _current_position + _aircraft.getSpeed() * (60.f / kilometer);
        checkHeight();
        _aircraft.calculateNewSpeed();
        _aircraft.checkSpeed();
        if (_plt)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            _plt->addPoint(_current_position[0], _current_position[1], _current_position[2]);
        }
    }
}

void Field::checkHeight()
{
    if (_current_position[2] > 11.5f && _aircraft.getSpeed()[2] > 0.f)
    {
        OurVector<3> new_speed = _aircraft.getSpeed();
        new_speed[2] /= 16;

        OurVector<3> new_acceleration = _aircraft.getAcceleration();
        new_acceleration[2] = new_acceleration[2] < 0.f ? new_acceleration[2] : -new_acceleration[2];

        _aircraft.setSpeed(new_speed);
        _aircraft.setAcceleration(new_acceleration);
    }
    if (_aircraft.getSpeed()[2] < -1.f)
    {
        OurVector<3> new_speed = _aircraft.getSpeed();
        new_speed[2] = 0;

        OurVector<3> new_acceleration = _aircraft.getAcceleration();
        new_acceleration[2] = 0;

        _aircraft.setSpeed(new_speed);
        _aircraft.setAcceleration(new_acceleration);
    }
}
