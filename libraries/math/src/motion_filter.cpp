#include <motion_filter.h>
#include <matrix.h>

static const double time_delta = 0.5;

MotionFilter::MotionFilter()
{
	OurMatrix<3, 3> state;
	state.setDiagonalValue(1);
	state[0][1] = time_delta;
	state[0][2] = time_delta * time_delta * 0.5;
	state[1][2] = time_delta;
	_filter.setStateMatrix(state);

	OurMatrix<3, 3> covariance_state;
	covariance_state.setDiagonalValue(10);
	_filter.setStateCovarianceMatrix(covariance_state);

	OurMatrix<3, 3> covariance_error;
	covariance_error.setDiagonalValue(3);
	_filter.setErrorCovarianceMatrix(covariance_error);

	OurMatrix<3, 3> covariance_noise;
	covariance_noise.setZero();
	_filter.setNoiseCovarianceMatrix(covariance_noise);

	OurMatrix<3, 1> observation_matrix;
	observation_matrix[0][0] = 1;
	_filter.setObservationMatrix(observation_matrix);
}

OurVector<3> MotionFilter::filter(OurVector<1> calculated_state)
{
	_filter.predict(time_delta);
	return _filter.correct(calculated_state);
}
