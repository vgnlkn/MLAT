#include <mlat_estimation.h>

//! Diagonals values for state covariance matrix(matrix P)
static const double k_covariance_dispersion[] = {1e4, 9000, 10, 1e4, 9000, 10, 1e4, 1e3, 10};

MlatEstimation::MlatEstimation()
{
    _filter.setStateCovarianceMatrix(getCovarianceStateMatrix());

    OurMatrix<k_dim_state, k_dim_state> covariance_error;
    _filter.setErrorCovarianceMatrix(covariance_error);

    OurMatrix<k_space_dim, k_space_dim> covariance_noise;
    covariance_noise.setDiagonalValue(k_covariance_dispersion[4]);
    _filter.setNoiseCovarianceMatrix(covariance_noise);

    OurMatrix<k_space_dim, k_dim_state> observation_matrix;
    observation_matrix.setZero();
    observation_matrix[0][0] = 1;
    observation_matrix[1][3] = 1;
    observation_matrix[2][6] = 1;
    _filter.setObservationMatrix(observation_matrix);
}


void MlatEstimation::updateStateMatrix(double time_delta)
{
    OurMatrix<k_dim_state, k_dim_state> state_matrix;
    state_matrix.setIdentity();
    for (uint8_t i = 0; i < k_dim_state; i += k_space_dim)
    {
        state_matrix[i][i + 1] = time_delta;
        state_matrix[i][i + 2] = time_delta * time_delta * 0.5;
        state_matrix[i + 1][i + 2] = time_delta;
    }
    _time_delta = time_delta;
    _filter.setStateMatrix(state_matrix);
}

void MlatEstimation::initState(OurVector<k_dim_state>& initial_state)
{
    _filter.setSystemVector(initial_state);
}

OurVector<9> MlatEstimation::estimatedState(OurVector<k_space_dim>& observation)
{
    _filter.predict();
    return _filter.correct(observation);
}

OurMatrix<k_dim_state, k_dim_state> MlatEstimation::getCovarianceStateMatrix()
{
    OurMatrix<k_dim_state, k_dim_state> covariance_state;
    for (uint8_t i = 0; i < k_dim_state; ++i)
    {
        covariance_state[i][i] = k_covariance_dispersion[i];
    }

    return covariance_state;
}