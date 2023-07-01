#ifndef MLAT_MAIN_CLASS_H
#define MLAT_MAIN_CLASS_H

class Field;
class Aircraft;
class IReader;
class IProduct;
class PlotterCreator;


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
    //! Plotters
    PlotterCreator* _plt_creator;

    IProduct* _plt_mlat;
    IProduct* _plt_real;
    IProduct* _plt_filter;
    IProduct* _plt_speed_filter;
    IProduct* _plt_speed_real;
    IProduct* _plt_acceleration_filter;
    IProduct* _plt_acceleration_real;
    IProduct* _plt_acceleration_standard_filter;
    IProduct* _plt_speed_standard_filter;
    IProduct* _plt_standard_filter;
    //! Reader
    IReader* _json_reader;
};


#endif //MLAT_MAIN_CLASS_H
