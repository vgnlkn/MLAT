#ifndef MLAT_TOWER_H
#define MLAT_TOWER_H

#include <cstring>
#include <cmath>
#include <vector.h>
#include <defines.h>

/*! \class Tower
*   \brief Tower class
*   This class describes the id and position of the tower.
*   It also calculates TOA for a specific tower.
*/
class Tower
{
public:
    //! Default constructor
    inline Tower() : _id(0) {};
    //! Copy constructor
    Tower(const Tower& other);
    //! Overloading operator=
    Tower& operator=(const Tower& other);
    //! Overloading operator==
    inline bool operator==(const Tower& other) const { return _id == other._id && _position == other._position; }
    //! Overloading operator<
    inline bool operator<(const Tower& other) const { return _id < other._id; }
    //! Receives a signal
    double calculateTime(const OurVector<k_space_dimension>& current_position);
    //! Getter for _position
    [[nodiscard]] inline OurVector<k_space_dimension> getPosition() const { return _position; }
    //! Setter for _position
    inline void setPosition(const OurVector<k_space_dimension>& other) { _position = other; }
    //! Getter for _id
    [[nodiscard]] inline uint16_t getID() const { return _id; }
    //! Setter for _id
    inline void setID(uint16_t id) { _id = id; }

private:
    //! Needed for operator=
    void swap(Tower& other);

private:
    //! id of the tower
    uint16_t _id;
    //! Position of tower
    OurVector<k_space_dimension> _position;
};


#endif //MLAT_TOWER_H
