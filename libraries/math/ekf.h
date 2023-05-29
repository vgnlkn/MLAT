#ifndef EKF_H
#define EKF_H

#include <matrix.h>
#include <defines.h>
#include <map>
#include <iomanip>
#include <fstream>

static const long double k_covariance_dispersion[] = { 1e9, 1e6, 1e3 };
// static const long double k_covariance_dispersion[] = { 1000, 1000, 1000 };

template <uint8_t dim_observation>
class EKF
{
public:
	//! Constructor
    EKF() = default;
	void init(std::map<uint16_t, OurVector<3>> tower_positions);
	OurVector<3> estimate(OurVector<dim_observation> observations);

protected:
	OurVector<dim_observation> equation(OurVector<3> x);
	void updateObservation();
private:
	OurVector<3> _state; //! (x, y, z)
	OurMatrix<3, 3> _evolution; //! Matrix F
	OurMatrix<3, 3> _covariance_state; //! Matrix P

	OurMatrix<dim_observation, 3> _observation_mtx; //! Matrix H
	OurMatrix<dim_observation, dim_observation> _observation_error; //! Matrix R

	//! Towers coordinates
	std::map<uint16_t, OurVector<3>> _tower_positions;


};

template <uint8_t dim_observation>
void EKF<dim_observation>::init(std::map<uint16_t, OurVector<3>> tower_positions)
{
	_tower_positions = tower_positions;
    _state.setValue(0);
	_evolution.setIdentity();

	_covariance_state.setZero();
    _observation_error.setDiagonalValue(1e-5);

	for (int i = 0; i < 3; ++i)
		_covariance_state[i][i] = k_covariance_dispersion[i];

}

template<uint8_t dim_observation>
inline OurVector<3> EKF<dim_observation>::estimate(OurVector<dim_observation> observations)
{
	_state = this->_evolution * _state;
	_covariance_state = _evolution * _covariance_state * _evolution.getTransposed();
	updateObservation();

	//! Проверить размерности
	OurMatrix<dim_observation, dim_observation> S = (_observation_mtx * _covariance_state) * _observation_mtx.getTransposed()
		+ _observation_error;
	OurMatrix<3, dim_observation> K = _covariance_state * _observation_mtx.getTransposed() * S.matrixInverse();
	// OurMatrix<3, dim_observation> K = _covariance_state * _observation_mtx.getTransposed() * S.getInverse();
    // std::cout << K << "\n\n";

	OurMatrix<3,3> I;
	I.setIdentity();

	_state = _state + K * (observations - equation(_state));
	_covariance_state = (I - K * _observation_mtx) * _covariance_state;

	return _state;
}

template<uint8_t dim_observation>
inline OurVector<dim_observation> EKF<dim_observation>::equation(OurVector<3> x)
{
    auto l2_norm = [](OurVector<3> vec) -> long double
    {
        return sqrtl(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    };

	OurVector<dim_observation> tdoa;
	int k = 0;
	for (int i = 0; i < TOWERS_COUNT; ++i)
	{
		for (int j = i + 1; j < TOWERS_COUNT; ++j)
		{
			tdoa[k++] = (1 / LIGHT_SPEED) * (l2_norm(x - _tower_positions[i]) - l2_norm(x - _tower_positions[j]));
		}
	}
	return tdoa;
}

template<uint8_t dim_observation>
inline void EKF<dim_observation>::updateObservation()
{
    auto l2_norm = [](OurVector<3> vec) -> long double
    {
        return sqrtl(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    };

	OurMatrix<dim_observation, 3> jacobi;
	auto jacobiRow = [&](OurVector<3> x, uint8_t i, uint8_t j)
	{
		OurVector<3> row;
		row.setValue(0);
		long double d_i = l2_norm(x - _tower_positions[i]);
		long double d_j = l2_norm(x - _tower_positions[j]);
		for (uint8_t col = 0; col < 3; ++col)
		{
			row[col] = (x[col] - _tower_positions[i][col]) / d_i -
					   (x[col] - _tower_positions[j][col]) / d_j;
		}
		return row;
	};
	int observation = 0;
	for (int i = 0; i < TOWERS_COUNT; ++i)
	{
		for (int j = i + 1; j < TOWERS_COUNT; ++j)
		{
			_observation_mtx[observation++] = jacobiRow(_state, i, j);
		}
	}
	_observation_mtx = (1 / LIGHT_SPEED) * _observation_mtx;
}

#endif