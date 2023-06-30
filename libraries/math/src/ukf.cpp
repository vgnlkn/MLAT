#include <ukf.h>

//! Diagonals values for state covariance matrix(matrix P)
static const double k_covariance_dispersion[] = { 1e3, 0.1, 1e3, 1e4, 0.1, 1e3, 1e3, 0.1, 1e4 };

OurMatrix<k_equations_count, k_observation_dim> UKF::getJacobian(OurVector<9>& position)
{
    OurMatrix<k_equations_count, k_observation_dim> jacobian;
    uint8_t k = 0;
    for (uint8_t i = 0; i < k_towers_count; ++i)
    {
        for (uint8_t j = i + 1; j < k_towers_count; ++j)
        {
            jacobian[k++] = getJacobianRow(position, i, j);
        }
    }
    return jacobian;
}

void UKF::setTowersCoordinates(std::map<uint16_t, OurVector<k_space_dimension>> tower_coordinates)
{
    _towers_coordinates = std::move(tower_coordinates);
}

void UKF::setInitialParams(const OurVector<k_observation_dim>& initial_coordinates)
{
    _initial_coordinates = initial_coordinates * 0.01f;

    _evolution.setZero();
    _evolution.setIdentity();
    for (int i = 0; i < k_observation_dim; i += k_space_dimension)
    {
        _evolution[i][i + 1] = k_sample_rate;
        _evolution[i][i + 2] = k_sample_rate * k_sample_rate * 0.5;
        _evolution[i + 1][i + 2] = k_sample_rate;
    }
    setCovarianceState();
    _observation_error.setDiagonalValue(k_noise_dispersion);
}

OurVector<9> UKF::solve(OurVector<k_equations_count>& tdoas)
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
    auto denominator = [=](uint8_t index, const OurVector<9>& coordinate)
    {
        return sqrt(
                std::pow(_towers_coordinates[index][0] - coordinate[0], 2) +
                std::pow(_towers_coordinates[index][1] - coordinate[3], 2) +
                std::pow(_towers_coordinates[index][2] - coordinate[6], 2)
        );
    };

    double denominator_i = denominator(tower_i, coordinate);
    double denominator_j = denominator(tower_j, coordinate);
    assert(denominator_i && denominator_j);

    for (int column = 0; column < k_observation_dim; column += k_space_dimension)
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
    for (uint8_t i = 0; i < k_towers_count; ++i)
    {
        for (uint8_t j = i + 1; j < k_towers_count; ++j)
        {
            if (_initial_tdoas[k] < 0)
            {
                jacobian[k] = -jacobian[k];
            }
        }

    }
    _observation_mtx = jacobian;
}

OurVector<k_equations_count> UKF::computeDiscrepancy()
{
    OurVector<k_equations_count> discrepancy;
    auto one_more_eq = [=](const OurVector<k_observation_dim>& at)
    {
        OurVector<3> x;
        x[0] = at[0];
        x[1] = at[3];
        x[2] = at[6];
        auto l2_norm = [](const OurVector<k_space_dimension>& vec) -> double
        {
            return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
        };

        OurVector<k_equations_count> tdoa;
        int k = 0;
        for (int i = 0; i < k_towers_count; ++i)
        {
            for (int j = i + 1; j < k_towers_count; ++j)
            {
                tdoa[k++] = (l2_norm(x - _towers_coordinates[i]) - l2_norm(x - _towers_coordinates[j]));
            }
        }
        return tdoa;
    };

    return one_more_eq(_initial_coordinates) - _initial_tdoas * k_light_speed;
}

void UKF::predict()
{
    _initial_coordinates = _evolution * _initial_coordinates;
    _covariance_state = _evolution * _covariance_state * _evolution.getTransposed();
}

void UKF::correct()
{
    OurMatrix<k_equations_count, k_equations_count> S = ((_observation_mtx * _covariance_state)
                                                        * _observation_mtx.getTransposed() + _observation_error);
    OurMatrix<k_observation_dim, k_equations_count> K = (_covariance_state * _observation_mtx.getTransposed())
                                                        * S.getLUPInverse();
    OurMatrix<k_observation_dim, k_observation_dim> I;
    I.setIdentity();
    _initial_coordinates = _initial_coordinates + K * computeDiscrepancy();
    _covariance_state = (I - K * _observation_mtx) * _covariance_state;
}

void UKF::setCovarianceState()
{
    _covariance_state.setZero();
    for (int i = 0; i < k_observation_dim; ++i)
    {
        _covariance_state[i][i] = k_covariance_dispersion[i];
    }
}
