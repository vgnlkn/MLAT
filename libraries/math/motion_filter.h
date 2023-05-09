#ifndef MOTION_FILTER_H
#define MOTION_FILTER_H

#include <kalman_filter.h>

class MotionFilter
{
public:
	MotionFilter();
	~MotionFilter() = default;
	void setInitial(OurVector<3>& init) { _filter.setSystemVector(init); }
	OurVector<3> filter(OurVector<1> calculated_state);

protected:
    void calculateStateMatrix(double time_delta);
    
private:
	//! Фильтр
	KalmanFilter<3, 1> _filter;
	OurMatrix<3, 3> _state;

};

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
	//~MlatEstimation() = default;
	//! Update state matrix
	void updateStateMatrix(double time_delta);
	//! Initial state for filter
	void initState(OurVector<9>& initial_state);
	//! Estimated state
	OurVector<9> estimatedState(OurVector<3>& observation);
	//! Resetes covariance matrixes;
	void reset();

private:
	//! Kalman Filter
	KalmanFilter<9, 3> _filter;
	//! Sample rate
	double _time_delta;
	
};

#endif