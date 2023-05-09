#include <motion_filter.h>
#include <matrix.h>

static const double time_delta = 1e-4;

MotionFilter::MotionFilter()
{
	OurMatrix<3, 3> covariance_state;
	covariance_state.setDiagonalValue(1e2);
	//covariance_state[2][2] = 1e-4;
	_filter.setStateCovarianceMatrix(covariance_state);

	OurMatrix<3, 3> covariance_error;
	_filter.setErrorCovarianceMatrix(covariance_error);

	OurMatrix<1, 1> covariance_noise;
	// эта матрица очень важна
	covariance_noise.setDiagonalValue(1e2);
	_filter.setNoiseCovarianceMatrix(covariance_noise);

	OurMatrix<1, 3> observation_matrix;
	observation_matrix.setZero();
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

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

MlatEstimation::MlatEstimation()
{
	//! P
	OurMatrix<9, 9> covariance_state;
	covariance_state.setDiagonalValue(1e4);
	_filter.setStateCovarianceMatrix(covariance_state);

	//! Q
	OurMatrix<9, 9> covariance_error;
	_filter.setErrorCovarianceMatrix(covariance_error);

	//! R
	OurMatrix<3, 3> covariance_noise;
	// эта матрица очень важна
	covariance_noise.setDiagonalValue(0.000001);
	_filter.setNoiseCovarianceMatrix(covariance_noise);

	//! H
	OurMatrix<3, 9> observation_matrix;
	observation_matrix.setZero();
	observation_matrix[0][0] = 1;
	observation_matrix[1][3] = 1;
	observation_matrix[2][6] = 1;
	_filter.setObservationMatrix(observation_matrix);

}


void MlatEstimation::updateStateMatrix(double time_delta)
{
	OurMatrix<9, 9> state_matrix;
	state_matrix.setIdentity();
	for (int i = 0; i < 9; i += 3)
	{
		state_matrix[i][i + 1] = time_delta;
		state_matrix[i][i + 2] = time_delta * time_delta * 0.5;
		state_matrix[i + 1][i + 2] = time_delta;
	}
	_time_delta = time_delta;
	_filter.setStateMatrix(state_matrix);
}

void MlatEstimation::initState(OurVector<9>& initial_state)
{
	_filter.setSystemVector(initial_state);
}

OurVector<9> MlatEstimation::estimatedState(OurVector<3>& observation)
{
	_filter.predict(_time_delta);
	return _filter.correct(observation);
}
