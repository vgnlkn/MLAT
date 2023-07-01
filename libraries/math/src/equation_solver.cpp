#include <equation_solver.h>


OurMatrix<k_equations_count, k_space_dim> EquationSolver::getJacobian(OurVector<3>& position)
{
    OurMatrix<k_equations_count, k_space_dim> jacobian;
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

void EquationSolver::setTowersCoordinates(std::map<uint16_t, OurVector<k_space_dim>> tower_coordinates)
{
    _towers_coordinates = std::move(tower_coordinates);
}

void EquationSolver::setInitialParams(const OurVector<k_space_dim>& initial_coordinates,
                                      const OurVector<k_equations_count>& initial_tdoas)
{
    _initial_coordinates = initial_coordinates;
    _initial_tdoas = initial_tdoas;
}

OurVector<k_space_dim> EquationSolver::solve(OurVector<k_equations_count>& tdoas)
{
    OurVector<k_equations_count> discrepancy;

    auto equation = [&](const OurVector<k_space_dim>& at, uint8_t tower_i, uint8_t tower_j)
    {
        auto coordinates_delta_i = _towers_coordinates[tower_i] - at;
        double d_i = 0;
        for (uint8_t i = 0; i < k_space_dim; ++i)
        {
            d_i += std::pow(coordinates_delta_i[i], 2);
        }
        d_i = std::sqrt(d_i);
        auto coordinates_delta_j = _towers_coordinates[tower_j] - at;
        double d_j = 0;
        for (uint8_t i = 0; i < k_space_dim; ++i)
        {
            d_j += std::pow(coordinates_delta_j[i], 2);
        }
        d_j = std::sqrt(d_j);
        return std::abs(d_i - d_j);
    };

    for (int iteration = 0; iteration < k_max_iterations_count; ++iteration)
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
                    _initial_tdoas[k] = -_initial_tdoas[k];
                }
                discrepancy[k] = equation(_initial_coordinates, i, j) - _initial_tdoas[k] * k_light_speed;
                k++;
            }
        }
        
        _initial_coordinates = _initial_coordinates + jacobian.pseudoInverse() * discrepancy;
    }
    _initial_tdoas = tdoas;
    return _initial_coordinates;
}

OurVector<k_space_dim> EquationSolver::getJacobianRow(OurVector<k_space_dim>& coordinate, uint8_t tower_i, uint8_t tower_j)
{
    OurVector<k_space_dim> jacobian_row;
    auto numerator = [](double tower_coordinate, double plane_coordinate) { return plane_coordinate - tower_coordinate; };
    auto denominator = [&](uint8_t index, const OurVector<k_space_dim>& coordinate_param)
    {
        return sqrt(
            std::pow(_towers_coordinates[index][0] - coordinate_param[0], 2) +
            std::pow(_towers_coordinates[index][1] - coordinate_param[1], 2) +
            std::pow(_towers_coordinates[index][2] - coordinate_param[2], 2)
        );
    };

    double denominator_i = denominator(tower_i, coordinate);
    double denominator_j = denominator(tower_j, coordinate);
    assert(static_cast<bool>(denominator_i) && static_cast<bool>(denominator_j));

    for (uint8_t column = 0; column < k_space_dim; ++column)
    {
        jacobian_row[column] = (numerator(coordinate[column], _towers_coordinates[tower_i][column]) / denominator_i -
                                numerator(coordinate[column], _towers_coordinates[tower_j][column]) / denominator_j);
    }
    return jacobian_row;
}

