#include <ukf.h>

static const double k_covariance_dispersion[] = { 1e3, 0.1, 1e3, 1e4, 0.1, 1e3, 1e3, 0.1, 1e4 };

OurMatrix<EQUATIONS_COUNT, 9> UKF::getJacobian(OurVector<9>& position)
{
    OurMatrix<EQUATIONS_COUNT, 9> jacobian;
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

void UKF::setTowersCoordinates(std::map<uint16_t, OurVector<3>> tower_coordinates)
{
    _towers_coordinates = std::move(tower_coordinates);
}

void UKF::setInitialParams(const OurVector<9>& initial_coordinates)
{
    _initial_coordinates = initial_coordinates * 0.01f;

    _evolution.setZero();
    _evolution.setIdentity();
    for (int i = 0; i < 9; i += 3)
    {
        _evolution[i][i + 1] = k_sample_rate;
        _evolution[i][i + 2] = k_sample_rate * k_sample_rate * 0.5;
        _evolution[i + 1][i + 2] = k_sample_rate;
    }
    setCovarianceState();
    _observation_error.setDiagonalValue(1e-6);
}

OurVector<9> UKF::solve(OurVector<EQUATIONS_COUNT>& tdoas)
{
    _initial_tdoas = tdoas;

    updateJacobian();
    predict();
    correct();

    return _initial_coordinates;
}

OurVector<9> UKF::getJacobianRow(OurVector<9>& coordinate, uint8_t tower_i, uint8_t tower_j)
{
    OurVector<9> jacobian_row;
    auto numerator = [](double tower_coordinate, double plane_coordinate) { return plane_coordinate - tower_coordinate; };
    auto denominator = [=](uint8_t index, double x, double y, double z)
    {
        return sqrt(
                std::pow(_towers_coordinates[index][0] - x, 2) +
                std::pow(_towers_coordinates[index][1] - y, 2) +
                std::pow(_towers_coordinates[index][2] - z, 2)
        );
    };

    double denominator_i = denominator(tower_i, coordinate[0], coordinate[3], coordinate[6]);
    double denominator_j = denominator(tower_j, coordinate[0], coordinate[3], coordinate[6]);
    assert(denominator_i && denominator_j);

    for (int column = 0; column < 9; column+=3)
    {
        jacobian_row[column] = (numerator(coordinate[column], _towers_coordinates[tower_i][column / 3]) / denominator_i -
                                numerator(coordinate[column], _towers_coordinates[tower_j][column / 3]) / denominator_j);
    }
    return jacobian_row;
}

void UKF::updateJacobian()
{
    auto jacobian = this->getJacobian(_initial_coordinates);
    uint8_t k = 0;
    for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
    {
        for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
        {
            if (_initial_tdoas[k] < 0)
            {
                jacobian[k] = -jacobian[k];
            }
        }

    }
    _observation_mtx = jacobian;
}

OurVector<EQUATIONS_COUNT> UKF::computeDiscrepancy()
{
    OurVector<EQUATIONS_COUNT> discrepancy;
    auto one_more_eq = [=](const OurVector<9>& at)
    {
        OurVector<3> x;
        x[0] = at[0];
        x[1] = at[3];
        x[2] = at[6];
        auto l2_norm = [](const OurVector<3>& vec) -> double
        {
            return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
        };

        OurVector<EQUATIONS_COUNT> tdoa;
        int k = 0;
        for (int i = 0; i < TOWERS_COUNT; ++i)
        {
            for (int j = i + 1; j < TOWERS_COUNT; ++j)
            {
                tdoa[k++] = (l2_norm(x - _towers_coordinates[i]) - l2_norm(x - _towers_coordinates[j]));
            }
        }
        return tdoa;
    };

    return one_more_eq(_initial_coordinates) - _initial_tdoas * LIGHT_SPEED;
}

void UKF::predict()
{
    _initial_coordinates = _evolution * _initial_coordinates;
    _covariance_state = _evolution * _covariance_state * _evolution.getTransposed();
}

void UKF::correct()
{
    OurMatrix<EQUATIONS_COUNT, EQUATIONS_COUNT> S = ((_observation_mtx * _covariance_state) * _observation_mtx.getTransposed() + _observation_error);
    OurMatrix<9, EQUATIONS_COUNT> K = (_covariance_state * _observation_mtx.getTransposed()) * S.getLUPInverse();
    OurMatrix<9,9> I;
    I.setIdentity();
    _initial_coordinates = _initial_coordinates + K * computeDiscrepancy();
    _covariance_state = (I - K * _observation_mtx) * _covariance_state;
}

void UKF::setCovarianceState()
{
    _covariance_state.setZero();
    for (int i = 0; i < 9; ++i)
    {
        _covariance_state[i][i] = k_covariance_dispersion[i];
    }
}
