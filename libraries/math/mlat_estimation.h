#ifndef MOTION_FILTER_H
#define MOTION_FILTER_H

#include <kalman_filter.h>


/*! \class MlatEstimation
*	\brief Kalman Filter implementation
*	This class necessery to calculate 
*	aircraft trajectory parameters using 
*	Kalman Filter. Such an implementation
*	doesn't calculate speed and acceleration
*	using instantaneous values of coordinates.
*	It makes an estimation which depends on 
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
	void initState(OurVector<9>& initial_state);
    //! Get default state covariance state matrix
    OurMatrix<9, 9> getCovarianceStateMatrix();
	//! Estimated state
	OurVector<9> estimatedState(OurVector<3>& observation);
	//! Resetes covariance matrixes;
	void reset() { _filter.setStateCovarianceMatrix(getCovarianceStateMatrix()); }

private:
	//! Kalman Filter
	KalmanFilter<9, 3> _filter;
	//! Sample rate
	double _time_delta;
};

#endif