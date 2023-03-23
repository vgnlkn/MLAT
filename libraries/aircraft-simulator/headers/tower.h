#ifndef MLAT_TOWER_H
#define MLAT_TOWER_H

#include <processor.h>
#include <vector.h>
#include <cstring>

class Tower
{
public:
    //! Default constructor
    Tower() = default;
    //! Copy constructor
    Tower(const Tower& other);
    //! Overloading operator=
    Tower& operator=(const Tower& other);

    //! Receives a signal
    // void acceptSignal(double time);

    //! Getter for _position
    OurVector<3> getPosition();
private:
    void swap(Tower& other);

private:
    OurVector<3> _position;
};


#endif //MLAT_TOWER_H
