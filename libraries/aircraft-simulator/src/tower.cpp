#include <tower.h>

Tower::Tower(const Tower &other)
{
    _position = other._position;
}

void Tower::swap(Tower &other)
{
    std::swap(_position, other._position);
}

Tower &Tower::operator=(const Tower &other)
{
    if (this != &other)
    {
        Tower(other).swap(*this);
    }

    return *this;
}

double Tower::calculateTime(const OurVector<3> &current_position)
{
    const OurVector<3>& b = _position;
    const OurVector<3>& a = current_position;

    double distance = sqrt((std::pow((a[0] - b[0]), 2)) + (std::pow((a[1] - b[1]), 2)) +
                               (std::pow((a[2] - b[2]), 2)));

    return distance / k_wave_speed;
}


