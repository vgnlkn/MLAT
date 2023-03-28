#include <equation_solver.h>
#include <cmath>
#include <cassert>

#define LIGHT_SPEED 3e5
#define MAX_ITERARATIONS_COUNT 1

OurMatrix<EQUATIONS_COUNT, 3> EquationSolver::getJacobian(OurVector<3>& position)
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

void EquationSolver::setTowersCoordinates(std::map<uint16_t, OurVector<3>> tower_coordinates)
{
    _towers_coordinates = tower_coordinates;
}

void EquationSolver::setInitialParams(const OurVector<3>& initial_coordinates,
                                      const OurVector<EQUATIONS_COUNT>& initial_tdoas)
{
    _initial_coordinates = initial_coordinates;
    _initial_tdoas = initial_tdoas;
}

OurVector<3> EquationSolver::solve(OurVector<EQUATIONS_COUNT>& tdoas)
{
    OurVector<EQUATIONS_COUNT> discrepancy;

    auto equation = [=](const OurVector<3>& at, uint8_t tower_i, uint8_t tower_j)
    {
        auto coordinates_delta_i = _towers_coordinates[tower_i] - at;
        double d_i = 0;
        for (uint8_t i = 0; i < 3; ++i)
        {
            d_i += std::pow(coordinates_delta_i[i], 2);
        }
        d_i = std::sqrt(d_i);
        auto coordinates_delta_j = _towers_coordinates[tower_j] - at;
        double d_j = 0;
        for (uint8_t i = 0; i < 3; ++i)
        {
            d_j += std::pow(coordinates_delta_j[i], 2);
        }
        d_j = std::sqrt(d_j);
        return std::abs(d_i - d_j);
    };

    for (int iteration = 0; iteration < MAX_ITERARATIONS_COUNT; ++iteration)
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
                    _initial_tdoas[k] = _initial_tdoas[k] < 0 ? -_initial_tdoas[k] : _initial_tdoas[k];
                }
                discrepancy[k] = equation(_initial_coordinates, i, j) - _initial_tdoas[k] * LIGHT_SPEED;
                k++;
            }
        }
        
        _initial_coordinates = _initial_coordinates + jacobian.pseudoInverse() * discrepancy;
    }
    _initial_tdoas = tdoas;
    return _initial_coordinates;
}

float EquationSolver::distance(const OurVector<3> from, const OurVector<3> to)
{
    return std::sqrt(std::pow(from[0] - to[0], 2) + std::pow(from[1] - to[1], 2) + std::pow(from[2] - to[2], 2));
}

OurVector<3> EquationSolver::getJacobianRow(OurVector<3>& coordinate, uint8_t tower_i, uint8_t tower_j)
{
    OurVector<3> jacobian_row;
    auto numerator = [](float tower_coordinate, float plane_coordinate) { return plane_coordinate - tower_coordinate; };
    auto denumerator = [=](uint8_t index, float x, float y, float z)
    {
        return std::sqrt(
            std::pow(_towers_coordinates[index][0] - x, 2) +
            std::pow(_towers_coordinates[index][1] - y, 2) +
            std::pow(_towers_coordinates[index][2] - z, 2)
        );
    };
    
    float denumerator_i = denumerator(tower_i, coordinate[0], coordinate[1], coordinate[2]);
    float denumerator_j = denumerator(tower_j, coordinate[0], coordinate[1], coordinate[2]);
    assert(denumerator_i && denumerator_j);

    for (int column = 0; column < 3; ++column)
    {
        jacobian_row[column] = (numerator(coordinate[column], _towers_coordinates[tower_i][column]) / denumerator_i -
            numerator(coordinate[column], _towers_coordinates[tower_j][column]) / denumerator_j);
    }
    return jacobian_row;
}

