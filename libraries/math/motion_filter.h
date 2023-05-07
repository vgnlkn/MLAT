#ifndef MOTION_FILTER_H
#define MOTION_FILTER_H

#include <kalman_filter.h>

class MotionFilter
{
public:
	MotionFilter();
	~MotionFilter() = default;

	OurVector<3> filter(OurVector<1> calculated_state);

protected:
    void calculateStateMatrix(double time_delta);
    


private:
	//! Фильтр
	KalmanFilter<3, 1> _filter;
	OurMatrix<3, 3> _state;

};

#endif