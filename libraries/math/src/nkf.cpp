#include <nkf.h>
#include <iomanip>

OurMatrix<EQUATIONS_COUNT, 3> NKF::getJacobian(OurVector<3>& position)
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

void NKF::setTowersCoordinates(std::map<uint16_t, OurVector<3>> tower_coordinates)
{
    _towers_coordinates = std::move(tower_coordinates);
}

void NKF::setInitialParams(const OurVector<3>& initial_coordinates,
                                      const OurVector<EQUATIONS_COUNT>& initial_tdoas)
{
    _initial_coordinates = initial_coordinates;

    _evolution.setZero();
    _evolution.setIdentity();
    _covariance_state.setZero();
	_covariance_state.setIdentity();
    _observation_error.setDiagonalValue(1e-6);
}

OurVector<3> NKF::solve(OurVector<EQUATIONS_COUNT>& tdoas)
{
    OurVector<EQUATIONS_COUNT> discrepancy;
    _initial_tdoas = tdoas;
    auto one_more_eq = [=](const OurVector<3>& at)
    {
        auto l2_norm = [](OurVector<3> vec) -> long double
        {
            return sqrtl(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
        };

        OurVector<EQUATIONS_COUNT> tdoa;
        int k = 0;
        for (int i = 0; i < TOWERS_COUNT; ++i)
        {
            for (int j = i + 1; j < TOWERS_COUNT; ++j)
            {
                tdoa[k++] = std::abs(l2_norm(at - _towers_coordinates[i]) - l2_norm(at - _towers_coordinates[j]));
            }
        }
        return tdoa;
    };

    auto jacobian = this->getJacobian(_initial_coordinates);
    uint8_t k = 0;
    for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
    {
        for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
        {
            if (_initial_tdoas[k] < 0)
            {
                jacobian[k] = -jacobian[k];
                _initial_tdoas[k] = _initial_tdoas[k] < 0 ? -_initial_tdoas[k] : _initial_tdoas[k];
            }
            k++;
        }
    }
    _observation_mtx = jacobian;

    _initial_coordinates = _evolution * _initial_coordinates;
    discrepancy = one_more_eq(_initial_coordinates) - _initial_tdoas * LIGHT_SPEED;
   
    //_covariance_state = _evolution * _covariance_state * _evolution.getTransposed();


    OurMatrix<EQUATIONS_COUNT, EQUATIONS_COUNT> S = (_observation_mtx * _covariance_state) * _observation_mtx.getTransposed();
		//+ _observation_error;
    //std::cout <<S << "\n\n";
    auto scp = S;
    auto sinv = scp.matrixInverse();
    for (int i=0; i<10; ++i)
    {
        for (int j=0; j<10; ++j)
        {
            if (sinv[i][j] != sinv[i][j])
            {
                
                std::cout << std::setprecision(40) << S.matrixInverse() << "\n\n";
              //  exit(0);
            }
        }
    }

    OurMatrix<3, EQUATIONS_COUNT> K = (_covariance_state * _observation_mtx.getTransposed()) * S.matrixInverse();

    _initial_coordinates = _initial_coordinates + _observation_mtx.pseudoInverse() * discrepancy;

    //exit(0);

    return _initial_coordinates;
}

long double NKF::distance(const OurVector<3>& from, const OurVector<3>& to)
{
    return sqrtl(std::pow(from[0] - to[0], 2) + std::pow(from[1] - to[1], 2) + std::pow(from[2] - to[2], 2));
}

OurVector<3> NKF::getJacobianRow(OurVector<3>& coordinate, uint8_t tower_i, uint8_t tower_j)
{
    OurVector<3> jacobian_row;
    auto numerator = [](long double tower_coordinate, long double plane_coordinate) { return plane_coordinate - tower_coordinate; };
    auto denominator = [=](uint8_t index, long double x, long double y, long double z)
    {
        return sqrtl(
            std::pow(_towers_coordinates[index][0] - x, 2) +
            std::pow(_towers_coordinates[index][1] - y, 2) +
            std::pow(_towers_coordinates[index][2] - z, 2)
        );
    };
    
    long double denominator_i = denominator(tower_i, coordinate[0], coordinate[1], coordinate[2]);
    long double denominator_j = denominator(tower_j, coordinate[0], coordinate[1], coordinate[2]);
    assert(denominator_i && denominator_j);

    for (int column = 0; column < 3; ++column)
    {
        jacobian_row[column] = (numerator(coordinate[column], _towers_coordinates[tower_i][column]) / denominator_i -
                                numerator(coordinate[column], _towers_coordinates[tower_j][column]) / denominator_j);
    }
    return jacobian_row;
}

