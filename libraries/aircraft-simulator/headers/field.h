#ifndef MLAT_FIELD_H
#define MLAT_FIELD_H

#include "aircraft.h"

const float kilometer = 1000.f;

class Field
{
public:
    //! Default constructor
    Field();
    //! Constructor with start position
    explicit Field(const OurVector<3>& start) : _current_position(start) {}
    //! Constructor with _aircraft
    explicit Field(const Aircraft& aircraft) : _aircraft(aircraft) {}

    //! Methods responsible for the movement of the _aircraft
    //! Start movement
    void startMovement();
    //! Stop movement
    void checkHeight();

private:
    //! Aircraft
    Aircraft _aircraft;
    //! Start position with (x; y; z) coordinates in vector
    OurVector<3> _current_position;
};


#endif
