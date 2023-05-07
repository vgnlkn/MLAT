#include <motion_filter.h>
#include <matrix.h>

static const double time_delta = 0.5;

MotionFilter::MotionFilter()
{

    OurMatrix<3, 3> state;
	_filter.setStateMatrix(state);

	OurMatrix<3, 3> covariance_state;
	_filter.setStateCovarianceMatrix(covariance_state);

	OurMatrix<3, 3> covariance_error;
	_filter.setErrorCovarianceMatrix(covariance_error);

	OurMatrix<1, 1> covariance_noise;
	_filter.setNoiseCovarianceMatrix(covariance_noise);

	OurMatrix<1, 3> observation_matrix;
	observation_matrix[0][0] = 1;
	_filter.setObservationMatrix(observation_matrix);

}

OurVector<3> MotionFilter::filter(OurVector<1> calculated_state)
{
	_filter.predict(time_delta);
	return _filter.correct(calculated_state);
}
