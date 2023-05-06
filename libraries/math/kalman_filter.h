#ifndef MLAT_KALMAN_FILTER_H
#define MLAT_KALMAN_FILTER_H

#include <matrix.h>

template<uint8_t N>
class KalmanFilter
{
public:
    KalmanFilter() = default;

    void setSystemVector(OurVector<N> other) { _system_vector = other; }

    void setStateMatrix(OurMatrix<N, N> other) { _state_transition_matrix = other; }
    void setObservationMatrix(OurMatrix<N, 1> other) { _observation_matrix = other; }
    void setErrorCovarianceMatrix(OurMatrix<N, N> other) { _error_covariance_matrix = other; }
    void setStateCovarianceMatrix(OurMatrix<N, N> other) { _state_covariance_matrix = other; }
    void setNoiseCovarianceMatrix(OurMatrix<N, N> other) { _noise_covariance_matrix = other; }

    void predict(double time_delta);
    OurVector<N> correct(const OurVector<3>& state_vector);
private:
    OurVector<N> _system_vector;              // x
    OurMatrix<N, N> _state_transition_matrix; // F
    OurMatrix<N, N> _error_covariance_matrix; // Q
    OurMatrix<N, N> _state_covariance_matrix; // P
    OurMatrix<N, 1> _observation_matrix;      // H
    OurMatrix<N, N> _noise_covariance_matrix; // R
};


template<uint8_t N>
void KalmanFilter<N>::predict(double time_delta)
{
    _system_vector = _state_transition_matrix * _system_vector + _observation_matrix * time_delta;
    _state_covariance_matrix = _state_transition_matrix *
            _state_covariance_matrix * _state_transition_matrix.getTransposed() + _error_covariance_matrix;

}


template<uint8_t N>
OurVector<N> KalmanFilter<N>::correct(const OurVector<3> &state_vector)
{
    OurMatrix<N, N> identity_matrix;
    identity_matrix.setIdentity();
    OurMatrix<N, N> S = _observation_matrix * _state_covariance_matrix * _observation_matrix.getTransposed()
            + _noise_covariance_matrix;
    OurMatrix<N, N> K = _state_covariance_matrix * _observation_matrix.getTransposed() * S.getInverse();
    OurVector<N> Y = state_vector - _observation_matrix * _system_vector;
    _system_vector = _system_vector + K * Y;
    _state_covariance_matrix = (identity_matrix - K * _observation_matrix) * _state_covariance_matrix;

    return _system_vector;
}


#endif //MLAT_KALMAN_FILTER_H