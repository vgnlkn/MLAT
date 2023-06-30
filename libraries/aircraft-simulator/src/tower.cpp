#include <tower.h>

Tower::Tower(const Tower &other)
{
    _position = other._position;
    _id = other._id;
}

void Tower::swap(Tower &other)
{
    std::swap(_position, other._position);
    std::swap(_id, other._id);
}

Tower &Tower::operator=(const Tower &other)
{
    if (this != &other)
    {
        Tower(other).swap(*this);
    }

    return *this;
}

double Tower::calculateTime(const OurVector<k_space_dimension> &current_position)
{
    const OurVector<k_space_dimension>& b = _position;
    const OurVector<k_space_dimension>& a = current_position;

    double distance = sqrt((std::pow((a[0] - b[0]), 2)) + (std::pow((a[1] - b[1]), 2)) +
                               (std::pow((a[2] - b[2]), 2)));

    return distance / k_light_speed;
}


