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
    //! Initialize the initial movement of the aircraft and towers
    void initialize();
    //! Calculates the new position of the aircraft
    void updateAircraftPosition();

    //! Methods to work with aircraft speed
    //! Stop movement
    void checkHeight();
    //! Deceleration in z
    void decreaseVerticalSpeed();
    //! Stop speed in z
    void stopVerticalSpeed();
    //! Update speed and check it
    void updateAircraftSpeed();

    //! Methods to work with towers
    //! Sets towers on the field
    void setTowers();
    //! Send signals to towers
    void sendSignalsToTowers(std::stack<float>& stack);
    //! Sets towers on the aircraft
    void setAircraftTowers();

    //! Methods to work with plotter
    //! Setting plotter
    void setPlotter(Plotter* plt) { _plt = plt; }
    //! Getter of _processor
    void updatePlot();
    //! Process Signals
    void processSignals() { _processor.process(); }
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
