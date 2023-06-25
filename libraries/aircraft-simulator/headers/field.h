#ifndef MLAT_FIELD_H
#define MLAT_FIELD_H

#include <chrono>
#include <thread>
#include <aircraft.h>
#include <processor.h>

/*! \class Field
*   \brief The class in which the flight is simulated.
*   The class in which the flight simulation of the aircraft is implemented.
*   The speed of its flight is also regulated here, towers are placed in the field
*   and towers to which the aircraft is connected.
*/
class Field
{
public:
    //! Default constructor
    Field() :
            _towers(new Tower[TOWERS_COUNT]),
            _plt_flight(nullptr),
            _plt_speed(nullptr),
            _plt_acceleration(nullptr),
            _tower_count(TOWERS_COUNT),
            _sample_rate(k_sample_rate)
    {
        _processor.setSampleRate(_sample_rate);
        _aircraft.setTimeDelta(_sample_rate);
    }
    //! Constructor with start position
    explicit Field(const OurVector<3>& start): 
        _towers(new Tower[TOWERS_COUNT]),
        _current_position(start), 
        _plt_flight(nullptr),
        _plt_speed(nullptr),
        _plt_acceleration(nullptr),
        _tower_count(TOWERS_COUNT),
        _sample_rate(k_sample_rate)
    {
        _processor.setSampleRate(_sample_rate);
        _aircraft.setTimeDelta(_sample_rate);
    }
    //! Constructor with aircraft
    explicit Field(const Aircraft& aircraft): 
        _aircraft(aircraft), 
        _towers(new Tower[TOWERS_COUNT]),
        _plt_flight(nullptr),
        _plt_speed(nullptr),
        _plt_acceleration(nullptr),
        _tower_count(TOWERS_COUNT),
        _sample_rate(k_sample_rate)
    {
        _processor.setSampleRate(_sample_rate);
        _aircraft.setTimeDelta(_sample_rate);
    }
    //! Destructor
    ~Field() { delete[] _towers; };

    //! Methods responsible for the movement of the _aircraft
    //! Setter for _aircraft
    void setAircraft(const Aircraft& aircraft) { _aircraft = aircraft; }
    //! Start movement
    [[noreturn]] void startMovement();
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
    void sendSignalsToTowers();
    //! Sets towers on the aircraft
    void setAircraftTowers();

    //! Methods to work with plotter
    //! Setting MLAT plotter
    void setPlotterMLAT(Plotter* plt) { _processor.setPlotterMlat(plt); }
    //! Setting Filter plotter
    void setPlotterFilter(Plotter* plt) { _processor.setPlotterFilter(plt); }
    //! Setting Filter plotter speed
    void setPlotterFilterSpeed(Plotter* plt) { _processor.setPlotterFilterSpeed(plt); }
    //! Setting Filter plotter acceleration
    void setPlotterFilterAcceleration(Plotter* plt) { _processor.setPlotterFilterAcceleration(plt); }
    //! Setting standard filter plotter
    void setPlotterStandardFilter(Plotter* plt) { _processor.setPlotterStandardFilter(plt); }
    //! Setting standard filter plotter speed
    void setPlotterFilterStandardSpeed(Plotter* plt) { _processor.setPlotterStandardFilterSpeed(plt); }
    //! Setting standard filter plotter acceleration
    void setPlotterFilterStandardAcceleration(Plotter* plt) { _processor.setPlotterStandardFilterAcceleration(plt); }
    //! Setting coordinate plotter with real values
    void setPlotterFlight(Plotter* plt) { _plt_flight = plt; }
    //! Setting speed plotter with real values
    void setPlotterSpeed(Plotter* plt) { _plt_speed = plt; }
    //! Setting acceleration plotter with real values
    void setPlotterAcceleration(Plotter* plt) { _plt_acceleration = plt; }
    //! Getter of _processor
    void updatePlot();

    //! Process Signals
    void processSignals(uint32_t i) { _processor.process(i); }
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
    //! Plotter object for coordinates
    Plotter* _plt_flight;
    //! Plotter object for speed
    Plotter* _plt_speed;
    //! Plotter object for acceleration
    Plotter* _plt_acceleration;
    //! Sample rate
    double _sample_rate;
};


#endif
