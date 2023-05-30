#ifndef EKF_H
#define EKF_H

#include <matrix.h>
#include <defines.h>
#include <map>
#include <iomanip>
#include <fstream>

static const long double k_covariance_dispersion[] = { 1e20, 1e20, 1e20 };
// static const long double k_covariance_dispersion[] = { 1000, 1000, 1000 };

template <uint8_t dim_observation>
class EKF
{
public:
	//! Constructor
    EKF() = default;
	void init(std::map<uint16_t, OurVector<3>> tower_positions);
	OurVector<3> estimate(OurVector<dim_observation> observations);

	OurVector<3> getJacobianRow(OurVector<3>& coordinate, uint8_t tower_i, uint8_t tower_j);
	OurMatrix<EQUATIONS_COUNT, 3> getJacobian(OurVector<3>& position);

	void setInitialParams(const OurVector<3>& initial_coordinates, const OurVector<EQUATIONS_COUNT>& initial_tdoas);

protected:
	OurVector<dim_observation> equationn(OurVector<3> x);
	void updateObservation();
private:
	OurVector<3> _state; //! (x, y, z)
	OurVector<dim_observation> _observation;
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
    _observation_error.setDiagonalValue(1e-6);

	_covariance_state.setIdentity();
	for (int i = 0; i < 3; ++i)
	{	
	//	_covariance_state[i][i] = k_covariance_dispersion[i];
	}


}

template<uint8_t dim_observation>
void EKF<dim_observation>::setInitialParams(const OurVector<3>& initial_coordinates,
                                      const OurVector<EQUATIONS_COUNT>& initial_tdoas)
{
    _state = initial_coordinates * (1.f / 16.f);
    _observation = initial_tdoas;
}


template<uint8_t dim_observation>
inline OurVector<3> EKF<dim_observation>::estimate(OurVector<dim_observation> observations)
{
	//_state = this->_evolution * _state;
	//_covariance_state = _evolution * _covariance_state * _evolution.getTransposed();
	updateObservation();

	//! Проверить размерности
	//OurMatrix<dim_observation, dim_observation> S = (_observation_mtx * _covariance_state) * _observation_mtx.getTransposed();
		//+ _observation_error;

	//std::cout << _observation_mtx << "\n\n";	
	//exit(0);
	//OurMatrix<3, dim_observation> K =  _observation_mtx.getTransposed() * S.pseudoInverse();
	//OurMatrix<3, dim_observation> K = (_covariance_state * _observation_mtx.getTransposed()) * S.matrixInverse();
    //std::cout << K << "\n\n";

	//OurMatrix<3,3> I;
	//I.setIdentity();

	//_state = _state + K * (observations - equation(_state));
	 auto equation = [&](const OurVector<3>& at, uint8_t tower_i, uint8_t tower_j)
    {
        auto coordinates_delta_i = _tower_positions[tower_i] - at;
        long double d_i = 0;
        for (uint8_t i = 0; i < 3; ++i)
        {
            d_i += std::pow(coordinates_delta_i[i], 2);
        }
        d_i = sqrtl(d_i);
        auto coordinates_delta_j = _tower_positions[tower_j] - at;
        long double d_j = 0;
        for (uint8_t i = 0; i < 3; ++i)
        {
            d_j += std::pow(coordinates_delta_j[i], 2);
        }
        d_j = sqrtl(d_j);
        return std::abs(d_i - d_j);
    };
	uint8_t k = 0;
	OurVector<EQUATIONS_COUNT> discrepancy;
	 for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
        {
            for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
            {
                if (observations[k] < 0)
                {
                    _observation_mtx[k] = -_observation_mtx[k];
                    observations[k] = observations[k] < 0 ? -observations[k] : observations[k];
                }
                discrepancy[k] = equation(_state, i, j) - observations[k] * LIGHT_SPEED;
                k++;
            }
        }
	_state = _state + _observation_mtx.pseudoInverse() * (discrepancy);
	//std::cout << _state << "\n\n";
	//_covariance_state = (I - K * _observation_mtx) * _covariance_state;

	return _state;
}

template<uint8_t dim_observation>
inline OurVector<dim_observation> EKF<dim_observation>::equationn(OurVector<3> x)
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
			tdoa[k++] = (l2_norm(x - _tower_positions[i]) - l2_norm(x - _tower_positions[j]));
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
	_observation_mtx = getJacobian(_state);
	//_observation_mtx = _observation_mtx;
}

template<uint8_t dim_observation>
OurVector<3> EKF<dim_observation>::getJacobianRow(OurVector<3>& coordinate, uint8_t tower_i, uint8_t tower_j)
{
    OurVector<3> jacobian_row;
    auto numerator = [](long double tower_coordinate, long double plane_coordinate) { return plane_coordinate - tower_coordinate; };
    auto denominator = [=](uint8_t index, long double x, long double y, long double z)
    {
        return sqrtl(
            std::pow(_tower_positions[index][0] - x, 2) +
            std::pow(_tower_positions[index][1] - y, 2) +
            std::pow(_tower_positions[index][2] - z, 2)
        );
    };
    
    long double denominator_i = denominator(tower_i, coordinate[0], coordinate[1], coordinate[2]);
    long double denominator_j = denominator(tower_j, coordinate[0], coordinate[1], coordinate[2]);
    assert(denominator_i && denominator_j);

    for (int column = 0; column < 3; ++column)
    {
        jacobian_row[column] = (numerator(coordinate[column], _tower_positions[tower_i][column]) / denominator_i -
                                numerator(coordinate[column], _tower_positions[tower_j][column]) / denominator_j);
    }
    return jacobian_row;
}

template<uint8_t dim_observation>
OurMatrix<EQUATIONS_COUNT, 3> EKF<dim_observation>::getJacobian(OurVector<3>& position)
{
    OurMatrix<EQUATIONS_COUNT, 3> jacobian;
    uint8_t k = 0;
    for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
    {
        for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
        {
            jacobian[k++] = getJacobianRow(position, i, j);
        }
    }
    return jacobian;
}


#endif