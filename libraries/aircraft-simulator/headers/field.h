#ifndef MLAT_FIELD_H
#define MLAT_FIELD_H

#include <aircraft.h>
#include <plotter.h>
#include <processor.h>

const float kilometer = 1000.f;

class Field
{
public:
    //! Default constructor
    Field();
    //! Constructor with start position
    explicit Field(const OurVector<3>& start): 
        _towers(new Tower[TOWERS_COUNT]),
        _current_position(start), 
        _plt(nullptr),
        _tower_count(TOWERS_COUNT) {}
    //! Constructor with aircraft
    explicit Field(const Aircraft& aircraft): 
        _aircraft(aircraft), 
        _towers(new Tower[TOWERS_COUNT]),
        _plt(nullptr),
        _tower_count(TOWERS_COUNT) {}
    //! Destructor
    ~Field();

    //! Methods responsible for the movement of the _aircraft
    //! Start movement
    void startMovement();
    //! Stop movement
    void checkHeight();
    //! Sets towers on the field
    void setTowers();
    //! Sets towers on the aircraft
    void setAircraftTowers();
    //! Setting plotter
    void setPlotter(Plotter* plt) { _plt = plt; }
    //! Getter of _processor
    [[nodiscard]] Processor getProcessor() const { return _processor; }
private:
    //! Aircraft
    Aircraft _aircraft;
    //! Start position with (x; y; z) coordinates in vector
    OurVector<3> _current_position;
    //! All towers in the field
    Tower* _towers;
    //! Number of towers
    uint16_t _tower_count;
    //! Class that manage tower time
    Processor _processor;
    //! Plotter object
    Plotter* _plt;
};


#endif
