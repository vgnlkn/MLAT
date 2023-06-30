#ifndef MLAT_AIRCRAFT_H
#define MLAT_AIRCRAFT_H

#include <matrix.h>
#include <tower.h>
#include <cmath>
#include <set>


/*! \class Aircraft
 *  \brief Class describing the aircraft.
 *  Describes the aircraft's airspeed, its acceleration
 *  and the towers connected to it.
*/
class Aircraft
{
public:
    //! Empty constructor
    inline Aircraft(): _time_delta(k_sample_rate) { _towers.reserve(k_towers_count); };
    //! Copy constructor
    Aircraft(const Aircraft& other);
    //! Constructor with acceleration
    explicit Aircraft(const OurVector<3>& acceleration);

    //! Methods to work with speed
    //! Get speed of the _aircraft
    [[nodiscard]] inline OurVector<k_space_dimension> getSpeed() const { return _state.getCol(0); }
    //! Get acceleration of the _aircraft
    [[nodiscard]] inline OurVector<k_space_dimension> getAcceleration() const { return _state.getCol(1); }
    //! Set speed of the _aircraft
    void setSpeed(const OurVector<k_space_dimension>& speed);
    //! Set acceleration of the _aircraft
    void setAcceleration(const OurVector<k_space_dimension>& acceleration);
    //! Calculate speed given acceleration
    void calculateNewSpeed();
    //! Check speed of the _aircraft
    void checkSpeed();
    //! Check acceleration of the _aircraft
    void checkAcceleration();
    //! Setter for time_delta
    void setTimeDelta(double dt) { _time_delta = dt; }

    //! Methods to work with signals
    //! Sends signal to tower
    double sendSignal(Tower tower, const OurVector<k_space_dimension>& current_position);

    //! Methods to work with towers
    //! Sets new tower
    void setNewTower(const Tower& tower) { _towers.push_back(tower); }
private:
    //! Towers that connected to the aircraft
    std::vector<Tower> _towers;
    //! first column - speed of the aircraft, second - acceleration
    OurMatrix<k_space_dimension, 2> _state;
    //! Time delta between measurements
    double _time_delta;
};


#endif
