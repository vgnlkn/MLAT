#ifndef MOTION_FILTER_H
#define MOTION_FILTER_H

#include <kalman_filter.h>

class MotionFilter
{
public:
	MotionFilter();
	~MotionFilter() = default;

	OurVector<3> filter(OurVector<3> calculated_state);

private:
	//! Фильтр
	KalmanFilter<3> _filter;

};

#endif