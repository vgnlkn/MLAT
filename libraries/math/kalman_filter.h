#ifndef MLAT_KALMAN_FILTER_H
#define MLAT_KALMAN_FILTER_H

#include <matrix.h>

/*! \class KalmanFilter
*   \brief Class describing Kalman Filter
* А class that implements a Kalman filter for evaluating output values
*/
template<uint8_t N>
class KalmanFilter
{
public:
    //! Default constructor
    KalmanFilter() = default;

    //! Setter for _system_vector
    void setSystemVector(const OurVector<N>& other) { _system_vector = other; }

    //! Setter for _state_transition_matrix
    void setStateMatrix(const OurMatrix<N, N>& other) { _state_transition_matrix = other; }
    //! Setter for _observation_matrix
    void setObservationMatrix(const OurMatrix<N, 1>& other) { _observation_matrix = other; }
    //! Setter for _error_covariance_matrix
    void setErrorCovarianceMatrix(const OurMatrix<N, N>& other) { _error_covariance_matrix = other; }
    //! Setter for _state_covariance_matrix
    void setStateCovarianceMatrix(const OurMatrix<N, N>& other) { _state_covariance_matrix = other; }
    //! Setter for _noise_covariance_matrix
    void setNoiseCovarianceMatrix(const OurMatrix<N, N>& other) { _noise_covariance_matrix = other; }

    //! Predicts model values
    void predict(double time_delta);
<<<<<<< HEAD
    OurVector<N> correct(const OurVector<3>& state_vector);
=======
    //! Corrects model values
    void correct(const OurVector<3>& state_vector);
>>>>>>> f1377ec12b14495a457c590ba927eecd74fdaec9
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
