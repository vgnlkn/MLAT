#ifndef EQUATION_SOLVER_H
#define EQUATION_SOLVER_H

#include <map>
#include <cmath>
#include <cassert>
#include <utility>
#include <matrix.h>
#include <defines.h>

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
    OurMatrix<k_equations_count, k_space_dimension> getJacobian(OurVector<k_space_dimension>& position);
    //! Setter for tower coordinates
    void setTowersCoordinates(std::map<uint16_t, OurVector<k_space_dimension>> tower_coordinates);
    //! Setter for initial parameters
    void setInitialParams(const OurVector<k_space_dimension>& initial_coordinates,
                          const OurVector<k_equations_count>& initial_tdoas);
    //! Method which return the solution
    OurVector<k_space_dimension> solve(OurVector<k_equations_count>& tdoas);

private:
    //! Returns the Jacobian row
    OurVector<k_space_dimension> getJacobianRow(OurVector<3>& position, uint8_t tower_i, uint8_t tower_j);

private:
    //! Initial coordinates
    OurVector<k_space_dimension> _initial_coordinates;
    //! Initial TDOAS
    OurVector<k_equations_count> _initial_tdoas;
    //! Tower coordinates
    std::map<uint16_t, OurVector<k_space_dimension>> _towers_coordinates;
};

#endif