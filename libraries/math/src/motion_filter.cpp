#include <motion_filter.h>
#include <matrix.h>




MlatEstimation::MlatEstimation()
{
	_filter.setStateCovarianceMatrix(getCovarianceStateMatrix());

	OurMatrix<9, 9> covariance_error;
	_filter.setErrorCovarianceMatrix(covariance_error);

	OurMatrix<3, 3> covariance_noise;
	covariance_noise.setDiagonalValue(k_covariance_noise);
	_filter.setNoiseCovarianceMatrix(covariance_noise);

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

OurMatrix<9, 9> MlatEstimation::getCovarianceStateMatrix()
{
    OurMatrix<9, 9> covariance_state;
    covariance_state[0][0] = k_state;
    covariance_state[1][1] = k_speed;
    covariance_state[2][2] = k_acceleration;
    covariance_state[3][3] = k_state;
    covariance_state[4][4] = k_speed;
    covariance_state[5][5] = k_acceleration;
    covariance_state[6][6] = k_state;
    covariance_state[7][7] = k_z_speed;
    covariance_state[8][8] = k_acceleration;
}
