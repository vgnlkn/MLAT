#ifndef MLAT_AIRCRAFT_H
#define MLAT_AIRCRAFT_H

#include <matrix.h>
#include <tower.h>
#include <cmath>

class Aircraft
{
public:
    //! Empty constructor
    Aircraft();
    //! Copy constructor
    Aircraft(const Aircraft& other);
    //! Constructor with acceleration
    explicit Aircraft(const OurVector<3>& acceleration);

    //! Methods to work with speed
    //! Get speed of the _aircraft
    OurVector<3> getSpeed() { return _state.getCol(0); }
    //! Get acceleration of the _aircraft
    OurVector<3> getAcceleration() { return _state.getCol(1); }
    //! Set speed of the _aircraft
    void setSpeed(const OurVector<3>& speed);
    //! Set acceleration of the _aircraft
    void setAcceleration(const OurVector<3>& acceleration);
    //! Calculate speed given acceleration
    void calculateNewSpeed();
    //! Check speed of the _aircraft
    void checkSpeed();
    //! Check acceleration of the _aircraft
    void checkAcceleration();
private:
    Tower* _towers;
    OurMatrix<3, 2> _state;
};


#endif
