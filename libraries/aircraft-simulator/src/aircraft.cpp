#include <aircraft.h>

Aircraft::Aircraft(const OurVector<3> &acceleration) :
    _time_delta(1.)
{
    _towers.reserve(TOWERS_COUNT);
    for (uint8_t i = 0; i < 3; ++i)
    {
        _state[i][1] = acceleration[i];
    }
}

Aircraft::Aircraft(const Aircraft &other) :
    _time_delta(1.f)
{
    _towers.reserve(TOWERS_COUNT);
    _state = other._state;
}

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
        _state[i][0] += _state[i][1] * _time_delta;
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
        if (_state[i][1] > 9.5f)
        {
            _state[i][1] = 2.5f;
        }
    }
}

double Aircraft::sendSignal(Tower tower, const OurVector<3>& current_position)
{
    return tower.calculateTime(current_position);
}
