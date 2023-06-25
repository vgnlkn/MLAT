#ifndef MLAT_MAIN_CLASS_H
#define MLAT_MAIN_CLASS_H

class Plotter;
class Field;
class Aircraft;

class MainClass
{
public:
    //! Constructor
    MainClass();
    //! Deconstructor
    ~MainClass();

    //! Run simulation of aircraft flight
    void runSimulation();
private:
    //! Set parameters of aircraft and field
    void setParameters();

private:
    Field* _field;
    Aircraft* _aircraft;
    Plotter* _plt_mlat;
    Plotter* _plt_real;
    Plotter* _plt_filter;
    Plotter* _plt_speed_filter;
    Plotter* _plt_speed_real;
    Plotter* _plt_acceleration_filter;
    Plotter* _plt_acceleration_real;
};


#endif //MLAT_MAIN_CLASS_H
