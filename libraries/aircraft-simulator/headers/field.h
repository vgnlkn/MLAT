#ifndef MLAT_FIELD_H
#define MLAT_FIELD_H

#include <aircraft.h>
#include <plotter.h>
#include <processor.h>
#include <chrono>
#include <thread>

static const double kilometer = 1000.f;
static const double k_sample_rate = 1e-2;


/*! \class Field
*   \brief The class in which the flight is simulated.
* The class in which the flight simulation of the aircraft is implemented.
* The speed of its flight is also regulated here, towers are placed in the field
* and towers to which the aircraft is connected.
*/
class Field
{
public:
    //! Default constructor
    Field();
    //! Constructor with start position
    explicit Field(const OurVector<3>& start): 
        _towers(new Tower[TOWERS_COUNT]),
        _current_position(start), 
        _plt_flight(nullptr),
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
        _tower_count(TOWERS_COUNT),
        _sample_rate(k_sample_rate)
    {
        _processor.setSampleRate(_sample_rate);
        _aircraft.setTimeDelta(_sample_rate);
    }
    //! Destructor
    ~Field() { if (_towers) delete[] _towers; };

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
    void sendSignalsToTowers();
    //! Sets towers on the aircraft
    void setAircraftTowers();

    //! Methods to work with plotter
    //! Setting MLAT plotter
    void setPlotterMLAT(Plotter* plt) { _processor.setPlotterMlat(plt); }
    //! Setting Filter plotter
    void setPlotterFilter(Plotter* plt) { _processor.setPlotterFilter(plt); }
    //! Setting flight plotter
    void setPlotterFlight(Plotter* plt) { _plt_flight = plt; }
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
    Plotter* _plt_flight;
    //! Sample rate
    double _sample_rate;
};


#endif
