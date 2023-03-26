#ifndef MLAT_PROCESSOR_H
#define MLAT_PROCESSOR_H

#include <chrono>
#include <vector>
#include <vector.h>
#include <map>
#include <set>
#include <tower.h>
#include <equation_solver.h>

/*! \class Processor
*   \brief Class manages TOA towers
*   Сlass that calculates TDOA, using the received TOA
*   from each of the towers. Also, this class is
*   associated with the class that solves the task.
*/
class Processor
{
public:
    Processor() = default;

    //! Adding TOA for one iteration
    void addTOA(uint16_t id, const std::stack<float>& TOA);
    //! Overloading operator[]
    std::stack<float>& operator[](uint16_t id) { return _towers_toa[id]; }

    //! Get tower using her id
    Tower getTower(uint16_t id) { return _towers[id]; }
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
    //std::vector<OurVector<6>> _TDOA;
};

#endif

