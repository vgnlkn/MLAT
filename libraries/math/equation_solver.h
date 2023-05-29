#ifndef EQUATION_SOLVER_H
#define EQUATION_SOLVER_H

#include <vector.h>
#include <matrix.h>
#include <defines.h>
#include <map>
#include <cmath>
#include <cassert>
#include <utility>

/*! \class EquationSolver
*   \brief Class which solves the MLAT problem
*   This class implements the Gauss-Newton algorithm
*   for the least squares problem
*/
class EquationSolver 
{
public:
    //! Constructor
    EquationSolver() = default;
    //! Destructor
    ~EquationSolver() = default;
    //! Returns the Jacobian of the least squares problem
    OurMatrix<EQUATIONS_COUNT, 3> getJacobian(OurVector<3>& position);
    //! Setter for tower coordinates
    void setTowersCoordinates(std::map<uint16_t, OurVector<3>> tower_coordinates);
    //! Setter for initial parameters
    void setInitialParams(const OurVector<3>& initial_coordinates, const OurVector<EQUATIONS_COUNT>& initial_tdoas);
    //! Method which return the solution
    OurVector<3> solve(OurVector<EQUATIONS_COUNT>& tdoas);

private:
    //! Calculates the distance between two dots
    long double distance(const OurVector<3>& from, const OurVector<3>& to);
    //! Returns the Jacobian row
    OurVector<3> getJacobianRow(OurVector<3>& position, uint8_t tower_i, uint8_t tower_j);

private:
    //! Initial coordinates
    OurVector<3> _initial_coordinates;
    //! Initial TDOAS
    OurVector<EQUATIONS_COUNT> _initial_tdoas;
    //! Tower coordinates
    std::map<uint16_t, OurVector<3>> _towers_coordinates;
};

#endif