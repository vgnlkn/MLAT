#ifndef UKF_H
#define UKF_H

#include <map>
#include <cmath>
#include <cassert>
#include <utility>
#include <matrix.h>
#include <defines.h>

static const uint8_t k_observation_dim = 9;

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
    OurMatrix<k_equations_count, k_observation_dim> getJacobian(OurVector<k_observation_dim>& position);
    //! Setter for tower coordinates
    void setTowersCoordinates(std::map<uint16_t, OurVector<k_space_dim>> tower_coordinates);
    //! Setter for initial parameters
    void setInitialParams(const OurVector<k_observation_dim>& initial_coordinates);
    //! Method which return the solution
    OurVector<k_observation_dim> solve(OurVector<k_equations_count>& tdoas);
    //! Reset state covariance matrix
    inline void reset() { setCovarianceState(); }

private:
    //! Returns the Jacobian row
    OurVector<k_observation_dim> getJacobianRow(OurVector<k_observation_dim>& position, uint8_t tower_i, uint8_t tower_j);
    //! Update sign of Jacobian based on tdoas
    void updateJacobian();
    //! Error recalculation
    OurVector<k_equations_count> computeDiscrepancy();
    //! Predicts model values
    void predict();
    //! Corrects model values
    void correct();
    //! Set covariance state matrix
    void setCovarianceState();

private:
    //! Initial coordinates
    OurVector<k_observation_dim> _initial_coordinates;
    //! Initial TDOAS
    OurVector<k_equations_count> _initial_tdoas;
    //! Tower coordinates
    std::map<uint16_t, OurVector<k_space_dim>> _towers_coordinates;

    OurVector<k_observation_dim> _state; //! (x, y, z)
	OurVector<k_equations_count> _observation;
	OurMatrix<k_observation_dim, k_observation_dim> _evolution; //! Matrix F
	OurMatrix<k_observation_dim, k_observation_dim> _covariance_state; //! Matrix P

	OurMatrix<k_equations_count, k_observation_dim> _observation_mtx; //! Matrix H
	OurMatrix<k_equations_count, k_equations_count> _observation_error; //! Matrix R
};

#endif