#ifndef UKF_H
#define UKF_H

#include <vector.h>
#include <matrix.h>
#include <defines.h>
#include <map>
#include <cmath>
#include <cassert>
#include <utility>

/*! \class UKF
*   \brief Class which solves the MLAT problem
*   This class implements the Gauss-Newton algorithm
*   for the least squares problem
*/
class UKF 
{
public:
    //! Constructor
    UKF() = default;
    //! Destructor
    ~UKF() = default;
    //! Returns the Jacobian of the least squares problem
    OurMatrix<EQUATIONS_COUNT, 9> getJacobian(OurVector<9>& position);
    //! Setter for tower coordinates
    void setTowersCoordinates(std::map<uint16_t, OurVector<3>> tower_coordinates);
    //! Setter for initial parameters
    void setInitialParams(const OurVector<9>& initial_coordinates);
    //! Method which return the solution
    OurVector<9> solve(OurVector<EQUATIONS_COUNT>& tdoas);

private:
    //! Returns the Jacobian row
    OurVector<9> getJacobianRow(OurVector<9>& position, uint8_t tower_i, uint8_t tower_j);
    //! Update sign of Jacobian based on tdoas
    void updateJacobian();
    //! Error recalculation
    OurVector<EQUATIONS_COUNT> computeDiscrepancy();
    //! Predicts model values
    void predict();
    //! Corrects model values
    void correct();

private:
    //! Initial coordinates
    OurVector<9> _initial_coordinates;
    //! Initial TDOAS
    OurVector<EQUATIONS_COUNT> _initial_tdoas;
    //! Tower coordinates
    std::map<uint16_t, OurVector<3>> _towers_coordinates;


    OurVector<9> _state; //! (x, y, z)
	OurVector<EQUATIONS_COUNT> _observation;
	OurMatrix<9, 9> _evolution; //! Matrix F
	OurMatrix<9, 9> _covariance_state; //! Matrix P

	OurMatrix<EQUATIONS_COUNT, 9> _observation_mtx; //! Matrix H
	OurMatrix<EQUATIONS_COUNT, EQUATIONS_COUNT> _observation_error; //! Matrix R
};

#endif