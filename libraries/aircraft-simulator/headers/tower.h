#ifndef MLAT_TOWER_H
#define MLAT_TOWER_H

#include <vector.h>
#include "../../../applications/utilities/defines.h"
#include <cstring>
#include <stack>
#include <cmath>

static const float waveSpeed = 300000.f;


/*! \class Tower
*   \brief Tower class
*   this class describes the id and position of the tower.
*   It also calculates TOA for a specific tower.
*/
class Tower
{
public:
    //! Default constructor
    Tower() = default;
    //! Copy constructor
    Tower(const Tower& other);
    //! Overloading operator=
    Tower& operator=(const Tower& other);

    //! Overloading operator==
    bool operator==(const Tower& other) const { return _id == other._id && _position == other._position; }
    //! Overloading operator<
    bool operator<(const Tower& other) const { return _id < other._id; }

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
    //! Needed for operator=
    void swap(Tower& other);

private:
    //! id of the tower
    uint16_t _id;
    //! Position of tower
    OurVector<3> _position;
};


#endif //MLAT_TOWER_H
