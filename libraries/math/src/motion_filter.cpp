#include <motion_filter.h>
#include <matrix.h>

static const double time_delta = 0.5;

MotionFilter::MotionFilter()
{
	OurMatrix<3, 3> covariance_state;
	covariance_state.setDiagonalValue(10);
	_filter.setStateCovarianceMatrix(covariance_state);

	OurMatrix<3, 3> covariance_error;
	_filter.setErrorCovarianceMatrix(covariance_error);

	OurMatrix<1, 1> covariance_noise;
	covariance_noise.setDiagonalValue(10);
	_filter.setNoiseCovarianceMatrix(covariance_noise);

	OurMatrix<1, 3> observation_matrix;
	observation_matrix[0][0] = 1;
	_filter.setObservationMatrix(observation_matrix);

}

OurVector<3> MotionFilter::filter(OurVector<1> calculated_state)
{
	this->calculateStateMatrix(time_delta);
	_filter.setStateMatrix(_state);
	_filter.predict(time_delta);
	return _filter.correct(calculated_state);
}

void MotionFilter::calculateStateMatrix(double time_delta)
{
	_state.setIdentity();
	_state[0][1] = time_delta;
	_state[1][2] = time_delta;
	_state[0][2] = 0.5 * time_delta * time_delta;
}
