#include <equation_solver.h>


OurMatrix<k_equations_count, k_space_dimension> EquationSolver::getJacobian(OurVector<3>& position)
{
    OurMatrix<k_equations_count, k_space_dimension> jacobian;
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

void EquationSolver::setTowersCoordinates(std::map<uint16_t, OurVector<k_space_dimension>> tower_coordinates)
{
    _towers_coordinates = std::move(tower_coordinates);
}

void EquationSolver::setInitialParams(const OurVector<k_space_dimension>& initial_coordinates,
                                      const OurVector<k_equations_count>& initial_tdoas)
{
    _initial_coordinates = initial_coordinates;
    _initial_tdoas = initial_tdoas;
}

OurVector<k_space_dimension> EquationSolver::solve(OurVector<k_equations_count>& tdoas)
{
    OurVector<k_equations_count> discrepancy;

    auto equation = [=](const OurVector<k_space_dimension>& at, uint8_t tower_i, uint8_t tower_j)
    {
        auto coordinates_delta_i = _towers_coordinates[tower_i] - at;
        double d_i = 0;
        for (uint8_t i = 0; i < k_space_dimension; ++i)
        {
            d_i += std::pow(coordinates_delta_i[i], 2);
        }
        d_i = sqrt(d_i);
        auto coordinates_delta_j = _towers_coordinates[tower_j] - at;
        double d_j = 0;
        for (uint8_t i = 0; i < k_space_dimension; ++i)
        {
            d_j += std::pow(coordinates_delta_j[i], 2);
        }
        d_j = sqrt(d_j);
        return std::abs(d_i - d_j);
    };

    for (uint8_t iteration = 0; iteration < k_max_iterations_count; ++iteration)
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
                discrepancy[k] = equation(_initial_coordinates, i, j) - _initial_tdoas[k] * k_light_speed;
                k++;
            }
        }
        
        _initial_coordinates = _initial_coordinates + jacobian.pseudoInverse() * discrepancy;
    }
    _initial_tdoas = tdoas;
    return _initial_coordinates;
}

OurVector<k_space_dimension> EquationSolver::getJacobianRow(OurVector<k_space_dimension>& coordinate, uint8_t tower_i, uint8_t tower_j)
{
    OurVector<k_space_dimension> jacobian_row;
    auto numerator = [](double tower_coordinate, double plane_coordinate) { return plane_coordinate - tower_coordinate; };
    auto denominator = [=](uint8_t index, const OurVector<k_space_dimension>& coordinate)
    {
        return sqrt(
                std::pow(_towers_coordinates[index][0] - coordinate[0], 2) +
                std::pow(_towers_coordinates[index][1] - coordinate[1], 2) +
                std::pow(_towers_coordinates[index][2] - coordinate[2], 2)
        );
    };

    double denominator_i = denominator(tower_i, coordinate);
    double denominator_j = denominator(tower_j, coordinate);
    assert(denominator_i && denominator_j);

    for (uint8_t column = 0; column < 3; ++column)
    {
        jacobian_row[column] = (numerator(coordinate[column], _towers_coordinates[tower_i][column]) / denominator_i -
                                numerator(coordinate[column], _towers_coordinates[tower_j][column]) / denominator_j);
    }
    return jacobian_row;
}

