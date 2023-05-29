#include <mlat_estimation.h>
#include <matrix.h>


static const long double array_dispersion[] = {1e4, 9000, 10, 1e4, 9000, 10, 1e4, 1e3, 10};

MlatEstimation::MlatEstimation()
{
	_filter.setStateCovarianceMatrix(getCovarianceStateMatrix());

	OurMatrix<9, 9> covariance_error;
	_filter.setErrorCovarianceMatrix(covariance_error);

	OurMatrix<3, 3> covariance_noise;
	covariance_noise.setDiagonalValue(array_dispersion[4]);
	_filter.setNoiseCovarianceMatrix(covariance_noise);

	OurMatrix<3, 9> observation_matrix;
	observation_matrix.setZero();
	observation_matrix[0][0] = 1;
	observation_matrix[1][3] = 1;
	observation_matrix[2][6] = 1;
	_filter.setObservationMatrix(observation_matrix);
}


void MlatEstimation::updateStateMatrix(long double time_delta)
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
    for (uint8_t i = 0; i < 9; ++i)
    {
        covariance_state[i][i] = array_dispersion[i];
    }

    return covariance_state;
}
