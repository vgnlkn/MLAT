#ifndef MLAT_KALMAN_FILTER_H
#define MLAT_KALMAN_FILTER_H

#include <matrix.h>
#include <iostream>

/*! \class KalmanFilter
*   \brief Class describing Kalman Filter
*   А class that implements a Kalman filter for evaluating output values
*/
template<uint8_t dim_state, uint8_t dim_observation>
class KalmanFilter
{
public:
    //! Default constructor
    KalmanFilter() = default;

    //! Setter for _system_vector
    void setSystemVector(const OurVector<dim_state>& other) { _system_vector = other; }

    //! Setter for _state_transition_matrix
    void setStateMatrix(const OurMatrix<dim_state, dim_state>& other) { _state_transition_matrix = other; }
    //! Setter for _observation_matrix
    void setObservationMatrix(const OurMatrix<dim_observation, dim_state>& other) { _observation_matrix = other; }
    //! Setter for _error_covariance_matrix
    void setErrorCovarianceMatrix(const OurMatrix<dim_state, dim_state>& other) { _error_covariance_matrix = other; }
    //! Setter for _state_covariance_matrix
    void setStateCovarianceMatrix(const OurMatrix<dim_state, dim_state>& other) { _state_covariance_matrix = other; }
    //! Setter for _noise_covariance_matrix
    void setNoiseCovarianceMatrix(const OurMatrix<dim_observation, dim_observation>& other) { _noise_covariance_matrix = other; }

    //! Predicts model values
    void predict(double time_delta);
    //! Corrects model values
    OurVector<dim_state> correct(const OurVector<dim_observation>& state_vector);
    virtual OurVector<dim_observation> getError(const OurVector<dim_observation> &state_vector);
protected:
    OurVector<dim_state> _system_vector;                                   // x
    OurMatrix<dim_state, dim_state> _state_transition_matrix;              // F
    OurMatrix<dim_state, dim_state> _error_covariance_matrix;              // Q
    OurMatrix<dim_state, dim_state> _state_covariance_matrix;              // P
    OurMatrix<dim_observation, dim_observation> _noise_covariance_matrix;  // R
    OurMatrix<dim_observation, dim_state> _observation_matrix;             // H
};

template<uint8_t dim_state, uint8_t dim_observation>
OurVector<dim_observation>
KalmanFilter<dim_state, dim_observation>::getError(const OurVector<dim_observation> &state_vector)
{
    return state_vector - (_observation_matrix * _system_vector);
}

template<uint8_t dim_state, uint8_t dim_observation>
void KalmanFilter<dim_state, dim_observation>::predict(double time_delta)
{
    _system_vector = _state_transition_matrix * _system_vector;
    _state_covariance_matrix = _state_transition_matrix * _state_covariance_matrix
                               * _state_transition_matrix.getTransposed() + _error_covariance_matrix;
}

template<uint8_t dim_state, uint8_t dim_observation>
OurVector<dim_state> KalmanFilter<dim_state, dim_observation>::correct(const OurVector<dim_observation> &state_vector)
{
    OurMatrix<dim_state, dim_state> identity_matrix;
    identity_matrix.setIdentity();
    OurMatrix<dim_observation, dim_observation> S = _observation_matrix * _state_covariance_matrix * _observation_matrix.getTransposed()
                                                    + _noise_covariance_matrix;
    OurMatrix<dim_state, dim_observation> K = _state_covariance_matrix * _observation_matrix.getTransposed() * S.getInverse();
    OurVector<dim_observation> Y = this->getError(state_vector);
    _system_vector = _system_vector + (K * Y);
    _state_covariance_matrix = (identity_matrix - K * _observation_matrix) * _state_covariance_matrix;

    return _system_vector;
}


#endif // MLAT_KALMAN_FILTER_H