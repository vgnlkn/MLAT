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

/*! \class Processor
*   \brief Class manages TOA towers
*   Сlass that calculates TDOA, using the received TOA
*   from each of the towers. Also, this class is
*   associated with the class that solves the task.
*/
class Processor
{
public:
    Processor()
    {
        // idk where we'll init plane position
        OurVector<3> init;
        // 0.00729437 0.0381568 0.0338469 0.0308624 0.0265525 0.00430991
        OurVector<6> init_tdoas;
        init_tdoas[0] = 0.00729437;
        init_tdoas[1] = 0.0381568;
        init_tdoas[2] = 0.0338469;
        init_tdoas[3] = 0.0308624;
        init_tdoas[4] = 0.0265525;
        init_tdoas[5] = 0.00430991;
        _solver.setInitialParams(init, init_tdoas);
    }

    //! Adding TOA for one iteration
    void addTOA(uint16_t id, const std::stack<float>& TOA);
    //! Overloading operator[]
    std::stack<float>& operator[](uint16_t id) { return _towers_toa[id]; }

    //! Get tower using her id
    Tower getTower(uint16_t id) { return _towers[id]; }

    void setTower(uint16_t id, const Tower& tower) 
    { 
        _towers[id] = tower; 
        _towers_coordinates[id] = tower.getPosition();
        _solver.setTowersCoordinates(_towers_coordinates);
    }

    void setPlotter(Plotter* plt)
    {
        _plt = plt;
    }

    //! Set tower in _towers using object of tower and tower's id
    void setTower(uint16_t id, const Tower& tower);


    /*! Processing accepted data
    * Calculating TDOA and getting aircraft position
    * using overdeterminated system of nonlinear equation
    * which solves using least squares method and
    * Gauss-Newton algorithm
    */
    void process();

    //! Getter for _TDOA;
    //[[nodiscard]] std::vector<OurVector<6>> getTDOA() const { return _TDOA; }
private:
    std::map<uint16_t, std::stack<float>> _towers_toa;
    std::map<uint16_t, Tower> _towers;
    std::map<uint16_t, OurVector<3>> _towers_coordinates;
    EquationSolver _solver;
    Plotter* _plt;
    //std::vector<OurVector<6>> _TDOA;
};

#endif

