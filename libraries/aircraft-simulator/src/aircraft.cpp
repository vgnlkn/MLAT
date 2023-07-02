#include <aircraft.h>

Aircraft::Aircraft(const OurVector<k_space_dim> &acceleration) :
    _time_delta(k_sample_rate)
{
    _towers.reserve(k_towers_count);
    for (uint8_t i = 0; i < k_space_dim; ++i)
    {
        _state[i][1] = acceleration[i];
    }
}

Aircraft::Aircraft(const Aircraft &other) :
    _time_delta(k_sample_rate)
{
    _towers.reserve(k_towers_count);
    _state = other._state;
}

void Aircraft::setSpeed(const OurVector<k_space_dim> &speed)
{
    for (uint8_t i = 0; i < k_space_dim; ++i)
    {
        _state[i][0] = speed[i];
    }
}

void Aircraft::setAcceleration(const OurVector<k_space_dim> &acceleration)
{
    for (uint8_t i = 0; i < k_space_dim; ++i)
    {
        _state[i][1] = acceleration[i];
    }
}

void Aircraft::calculateNewSpeed()
{
    for (uint8_t i = 0; i < k_space_dim; ++i)
    {
        _state[i][0] += _state[i][1] * _time_delta;
    }
}

void Aircraft::checkSpeed() {
    for (uint8_t i = 0; i < k_space_dim; ++i)
    {
        if (_state[i][0] > k_max_speed)
        {
            _state[i][1] = 0.f;
            _state[i][0] = k_max_speed;
        }
    }
}

void Aircraft::checkAcceleration()
{
    for (uint8_t i = 0; i < k_space_dim; ++i)
    {
        if (_state[i][1] > k_max_acceleration)
        {
            _state[i][1] = k_average_acceleration;
        }
    }
}

double Aircraft::sendSignal(Tower tower, const OurVector<k_space_dim>& current_position)
{
    return tower.calculateTime(current_position);
}

Aircraft &Aircraft::operator=(const Aircraft &other)
{
    if (this != &other)
    {
        Aircraft(other).swap(*this);
    }

    return *this;
}

void Aircraft::swap(Aircraft &other)
{
    std::swap(other._state, this->_state);
    std::swap(other._time_delta, this->_time_delta);
    std::swap(other._towers, this->_towers);
}
