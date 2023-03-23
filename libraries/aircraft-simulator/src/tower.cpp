#include "../headers/tower.h"

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


OurVector<3> Tower::getPosition() {
    return _position;
}
