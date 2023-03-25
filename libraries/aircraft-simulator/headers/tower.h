#ifndef MLAT_TOWER_H
#define MLAT_TOWER_H

#include <vector.h>
#include <cstring>
#include <stack>
#include <cmath>

static const float waveSpeed = 300000.f;

class Tower
{
public:
    //! Default constructor
    Tower() = default;
    //! Copy constructor
    Tower(const Tower& other);
    //! Overloading operator==
    bool operator==(const Tower& other) const { return _id == other._id && _position == other._position; }
    //! Overloading operator=
    Tower& operator=(const Tower& other);
    //! Receives a signal
    float calculateTime(const OurVector<3>& current_position);
    //! Getter for _position
    [[nodiscard]] OurVector<3> getPosition() const { return _position; }
    //! Setter for _position
    void setPosition(const OurVector<3>& other) { _position = other; }
    //! Getter for _id
    [[nodiscard]] uint16_t getID() const { return _id; }
    //! Setter for _id
    void setID(uint16_t id) { _id = id; }
private:
    void swap(Tower& other);

private:
    uint16_t _id;
    OurVector<3> _position;
};


#endif //MLAT_TOWER_H
