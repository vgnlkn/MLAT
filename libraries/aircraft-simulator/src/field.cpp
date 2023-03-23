#include <field.h>
#include <aircraft.h>

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
    for(uint16_t i = 0; i < 1000; ++i)
    {
        _current_position = _current_position + _aircraft.getSpeed() * (60.f / kilometer);
        checkHeight();
        _aircraft.calculateNewSpeed();
        _aircraft.checkSpeed();
        _times[i] = 60.f * i;
        _coordinates[i] = _current_position;
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
