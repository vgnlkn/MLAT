#ifndef MLAT_PROCESSOR_H
#define MLAT_PROCESSOR_H

#include <chrono>
#include <vector>
#include <vector.h>
#include <map>
#include <set>
#include <tower.h>
#include <equation_solver.h>
#include <plotter.h>
#include <random>
#include <mlat_estimation.h>

/*! \class NoiseGenerator
*   \brief Generate noise with normal distribution
*/
class NoizeGenerator
{
public:
    inline NoizeGenerator(): _rd(), _gen(_rd()), _distribution(0, 1e-6) {}
    inline double generate() { return _distribution(_gen); }

private:
    std::random_device _rd;
    std::mt19937 _gen;
    std::normal_distribution<double> _distribution;
};

/*! \class Processor
*   \brief Class manages TOA towers
*   Сlass that calculates TDOA, using the received TOA
*   from each of the towers. Also, this class is
*   associated with the class that solves the task.
*/
class Processor
{
public:
    //! Constructor
    inline Processor() : _plt_filter(nullptr), _plt_mlat(nullptr), _plt_filter_acceleration(nullptr),
    _plt_filter_speed(nullptr), _noise(new NoizeGenerator), _iteration(1), _overstatement(0) {}
    //! Destructor
    inline ~Processor() { if (_noise) { delete _noise; } }
    //! Initialize solver
    void initSolver();
    //! Adding TOA for one iteration
    void addTOA(uint16_t id, double TOA);
    //! Calculates TDOA
    void calculateTDOA(OurVector<EQUATIONS_COUNT>& tdoas);
    //! Overloading operator[]
    double& operator[](uint16_t id) { return _towers_toa[id]; }
    //! Get tower using her id
    Tower getTower(uint16_t id) { return _towers[id]; }
    //! Setter for _plt_mlat
    void setPlotterMlat(Plotter* plt) { _plt_mlat = plt; }
    //! Setter for _plt_filter
    void setPlotterFilter(Plotter* plt) { _plt_filter = plt; }
    //! Setter for _plt_filter_speed
    void setPlotterFilterSpeed(Plotter* plt) { _plt_filter_speed = plt; }
    //! Setter for _plt_filter_acceleration
    void setPlotterFilterAcceleration(Plotter* plt) { _plt_filter_acceleration = plt; }
    //! Set tower in _towers using object of tower and tower's id
    void setTower(uint16_t id, const Tower& tower);
    //! Set samplerate
    void setSampleRate(double sample_rate);

    /*! Processing accepted data
    * Calculating TDOA and getting aircraft position
    * using overdeterminated system of nonlinear equation
    * which solves using least squares method and
    * Gauss-Newton algorithm
    */
    void process(uint32_t iter);
    
private:
    //! TOA
    std::map<uint16_t, double> _towers_toa;
    //! Towers
    std::map<uint16_t, Tower> _towers;
    //! Towers position
    std::map<uint16_t, OurVector<3>> _towers_coordinates;
    //! Solver for non-linear equation system
    EquationSolver _solver;
    //! Object which draws plots with direct problem
    Plotter* _plt_mlat;
    //! Object which draws plots with inverse problem
    Plotter* _plt_filter;
    //! Plotter for filter speed
    Plotter* _plt_filter_speed;
    //! Plotter for filter acceleration
    Plotter* _plt_filter_acceleration;
    //! Noise generator
    NoizeGenerator* _noise;
    //! Estimation
    MlatEstimation _estim;
    //! Average coordinates
    OurVector<3> _mlat_average, _kalman_average;
    //! Vectors, necessery to calculate amplitude
    OurVector<3> _mlat_min, _mlat_max;
    //! Counter for iterations
    uint32_t _iteration;
    //! Overstatement
    uint32_t _overstatement;

};


#endif

