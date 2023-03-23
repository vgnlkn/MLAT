#include <aircraft.h>

Aircraft::Aircraft(const OurVector<3> &acceleration) : _towers(new Tower[4])
{
    for (uint8_t i = 0; i < 3; ++i)
    {
        _state[i][1] = acceleration[i];
    }
}

Aircraft::Aircraft(const Aircraft &other)
{
    _state = other._state;

    _towers = new Tower[4];
    for (uint8_t i = 0; i < 4; ++i)
    {
        _towers[i] = other._towers[i];
    }
}


Aircraft::Aircraft() : _towers(new Tower[4]) {}

void Aircraft::setSpeed(const OurVector<3> &speed)
{
    for (uint8_t i = 0; i < 3; ++i)
    {
        _state[i][0] = speed[i];
    }
}

void Aircraft::setAcceleration(const OurVector<3> &acceleration)
{
    for (uint8_t i = 0; i < 3; ++i)
    {
        _state[i][1] = acceleration[i];
    }
}

void Aircraft::calculateNewSpeed()
{
    for (uint8_t i = 0; i < 3; ++i)
    {
        _state[i][0] += _state[i][1];
    }
}

void Aircraft::checkSpeed() {
    for (uint8_t i = 0; i < 3; ++i)
    {
        if (_state[i][0] > 250.f)
        {
            _state[i][1] = 0.f;
            _state[i][0] = 250.f;
        }
    }
}

void Aircraft::checkAcceleration()
{
    for (uint8_t i = 0; i < 3; ++i)
    {
        if (_state[i][1] > 10.f)
        {
            _state[i][1] = 2.5f;
        }
    }
}

void Aircraft::sendData(const OurVector<3>& coordinates, float time)
{
    _towers[0].acceptSignal(time);
    _towers[0].acceptCoordinates(coordinates);
}

