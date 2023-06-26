#ifndef MLAT_MAIN_CLASS_H
#define MLAT_MAIN_CLASS_H

class Plotter;
class Field;
class Aircraft;
class IReader;


/*! \class MainClass
*   \brief Run model and plotters
* 	It creates model objects, activates it and adds plotters.
*/
class MainClass
{
public:
    //! Constructor
    MainClass();
    //! Destructor
    ~MainClass();

    //! Run simulation of aircraft flight
    void runSimulation();
private:
    //! Set parameters of aircraft and field
    void setParameters();

private:
    //! Model
    Field* _field;
    Aircraft* _aircraft;
    //! Graphics
    Plotter* _plt_mlat;
    Plotter* _plt_real;
    Plotter* _plt_filter;
    Plotter* _plt_speed_filter;
    Plotter* _plt_speed_real;
    Plotter* _plt_acceleration_filter;
    Plotter* _plt_acceleration_real;
    Plotter* _plt_acceleration_standard_filter;
    Plotter* _plt_speed_standard_filter;
    Plotter* _plt_standard_filter;
    //! Reader
    IReader* _json_reader;
};


#endif //MLAT_MAIN_CLASS_H
