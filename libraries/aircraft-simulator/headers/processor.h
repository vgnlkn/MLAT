#ifndef MLAT_PROCESSOR_H
#define MLAT_PROCESSOR_H

class Plotter;

#include <chrono>
#include <vector>
#include <map>
#include <set>
#include <tower.h>
#include <equation_solver.h>
#include <random>
#include <ukf.h>
#include <mlat_estimation.h>


/*! \class NoiseGenerator
*   \brief Generate noise with normal distribution
*/
class NoizeGenerator
{
public:
    inline NoizeGenerator(): _rd(), _gen(_rd()), _distribution(0, k_noise_dispersion) {}
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
    Processor();
    //! Destructor
    inline ~Processor() { delete _noise; }

    //! Initialize solver
    void initSolver();
    //! Adding TOA for one iteration
    inline void addTOA(uint16_t id, double TOA) { _towers_toa[id] = TOA; }
    //! Calculates TDOA
    void calculateTDOA(OurVector<k_equations_count>& tdoas);
    //! Overloading operator[]
    inline double& operator[](uint16_t id) { return _towers_toa[id]; }
    //! Setter for _plt_mlat
    inline void setPlotterMlat(Plotter* plt) { _plt_mlat = plt; }
    //! Setter for _plt_filter
    inline void setPlotterFilter(Plotter* plt) { _plt_filter = plt; }
    //! Setter for _plt_filter_speed
    inline void setPlotterFilterSpeed(Plotter* plt) { _plt_filter_speed = plt; }
    //! Setter for _plt_filter_acceleration
    inline void setPlotterFilterAcceleration(Plotter* plt) { _plt_filter_acceleration = plt; }
    //! Setter for _plt_standard_filter
    inline void setPlotterStandardFilter(Plotter* plt) { _plt_standard_filter = plt; }
    //! Setter for _plt_standard_filter_speed
    inline void setPlotterStandardFilterSpeed(Plotter* plt) { _plt_standard_filter_speed = plt; }
    //! Setter for _plt_standard_filter_acceleration
    inline void setPlotterStandardFilterAcceleration(Plotter* plt) { _plt_standard_filter_acceleration = plt; }
    //! Set tower in _towers using object of tower and tower's id
    void setTower(uint16_t id, const Tower& tower);
    //! Update state covariance matrix(matrix P)
    inline void setSampleRate(double time_delta) { _estim.updateStateMatrix(time_delta); }

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
    std::map<uint16_t, OurVector<k_space_dim>> _towers_coordinates;
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
    //! Plotter for standard filter speed
    Plotter* _plt_standard_filter_speed;
    //! Plotter for standard filter acceleration
    Plotter* _plt_standard_filter_acceleration;
    //! Plotter for standard filter coordinates
    Plotter* _plt_standard_filter;
    //! Noise generator
    NoizeGenerator* _noise;
    //! Average coordinates
    OurVector<k_space_dim> _mlat_average, _kalman_average;
    //! Vectors, necessery to calculate amplitude
    OurVector<k_space_dim> _mlat_min, _mlat_max;
    //! Counter for iterations
    uint32_t _iteration;
    //! Overstatement
    uint32_t _overstatement;
    //! Kalman Filter with built-in LSM
    UKF _unscented_filter;
    //! Estimation for standard Kalman filter
    MlatEstimation _estim;
};


#endif

