#ifndef MOTION_FILTER_H
#define MOTION_FILTER_H

#include <kalman_filter.h>
#include <defines.h>

static const uint8_t k_dim_state = 9;

/*! \class MlatEstimation
*	\brief Kalman Filter implementation
*	This class necessary to calculate aircraft trajectory parameters using
*	Kalman Filter. Such an implementation doesn't calculate speed and acceleration
*	using instantaneous values of coordinates. It makes an estimation which depends on
*	previous calculations and new observations.
*/
class MlatEstimation
{
public:
    //! Constructor
    MlatEstimation();
    //! Destructor
    ~MlatEstimation() = default;
    //! Update state matrix
    void updateStateMatrix(double time_delta);
    //! Initial state for filter
    void initState(OurVector<k_dim_state>& initial_state);
    //! Get default state covariance state matrix
    OurMatrix<k_dim_state, k_dim_state> getCovarianceStateMatrix();
    //! Estimated state
    OurVector<k_dim_state> estimatedState(OurVector<k_space_dim>& observation);
    //! Resets covariance matrixes;
    void reset() { _filter.setStateCovarianceMatrix(getCovarianceStateMatrix()); }

private:
    //! Kalman Filter
    KalmanFilter<k_dim_state, k_space_dim> _filter;
    //! Sample rate
    double _time_delta;
};

#endif