#include "../headers/field.h"
#include "../headers/aircraft.h"

Field::Field()
{
    _current_position[2] = 10;
}

void Field::startMovement()
{
    _aircraft.checkAcceleration();
    for(uint16_t i = 0; i < 1000; ++i)
    {
        _current_position = _current_position + _aircraft.getSpeed() * (60.f / kilometer);
        checkHeight();
        _aircraft.calculateNewSpeed();
        _aircraft.checkSpeed();
        _aircraft.sendData(_current_position, 60.f * i);
        // std::cout << "current _aircraft position: " << _current_position  << " with time: " << 60.f * i << '\n';
    }
}

void Field::checkHeight()
{
    if (_current_position[2] > 11.5f && _aircraft.getSpeed()[2] > 0.f)
    {
        OurVector<3> new_speed = _aircraft.getSpeed();
        new_speed[2] = 0;

        OurVector<3> new_acceleration = _aircraft.getAcceleration();
        new_acceleration[2] = 0;

        _aircraft.setSpeed(new_speed);
        _aircraft.setAcceleration(new_acceleration);
    }
}
