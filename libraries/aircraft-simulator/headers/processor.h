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
#include <motion_filter.h>

/*! \class NoiseGenerator
*   \brief Generate noise with normal distribution
*/
class NoizeGenerator
{
public:
    inline NoizeGenerator(): _rd(), _gen(_rd()), _distribution(0, 0.000001) {}
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
    inline Processor() : _plt(nullptr), _noise(new NoizeGenerator) {}
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
    //! Setter for _plt
    void setPlotter(Plotter* plt) { _plt = plt; }
    //! Adds a point to the graph
    void addPoint(const OurVector<3>& coords);

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
    void process();
private:
    //! TOA
    std::map<uint16_t, double> _towers_toa;
    //! Towers
    std::map<uint16_t, Tower> _towers;
    //! Towers position
    std::map<uint16_t, OurVector<3>> _towers_coordinates;
    //! Solver for non-linear equation system
    EquationSolver _solver;
    //! Object which draws plots
    Plotter* _plt;
    //! Noise generator
    NoizeGenerator* _noise;
    //! Estimation
    MlatEstimation _estim;
};


#endif

