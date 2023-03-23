#ifndef MLAT_FIELD_H
#define MLAT_FIELD_H

#include <aircraft.h>

const float kilometer = 1000.f;

class Field
{
public:
    //! Default constructor
    Field();
    //! Constructor with start position
    explicit Field(const OurVector<3>& start)
            :  _coordinates(new OurVector<3>[1000]), _times(new float[1000]), _current_position(start) {}
    //! Constructor with aircraft
    explicit Field(const Aircraft& aircraft)
            : _aircraft(aircraft), _coordinates(new OurVector<3>[1000]), _times(new float[1000]) {}
    //! Destructor
    ~Field();

    //! Methods responsible for the movement of the _aircraft
    //! Start movement
    void startMovement();
    //! Stop movement
    void checkHeight();

    //! Returns element of _times
    float getTime(uint16_t i) { return _times[i]; };
    //! Returns element of _coordinates
    OurVector<3> getCoordinate(uint16_t i) { return _coordinates[i]; }

private:
    //! Aircraft
    Aircraft _aircraft;
    //! Start position with (x; y; z) coordinates in vector
    OurVector<3> _current_position;
    //! Arrays of data
    OurVector<3>* _coordinates;
    float* _times;
};


#endif
