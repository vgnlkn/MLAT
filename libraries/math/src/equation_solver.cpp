#include <equation_solver.h>
#include <cmath>
#include <cassert>

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

auto EquationSolver::solve(OurVector<EQUATIONS_COUNT>& tdoas)
{
    for (uint16_t i = 0; i < TOWERS_COUNT; ++i)
    {
        for (uint16_t j = i + 1; j < TOWERS_COUNT - 1; ++j)
        {
            //tdoas[k++] = std::abs(_towers_toa[i].top() - _towers_toa[j].top());
        }
    }
}

float EquationSolver::distance(const OurVector<3> from, const OurVector<3> to)
{
    return std::sqrt(std::pow(from[0] - to[0], 2) + std::pow(from[1] - to[1], 2) + std::pow(from[2] - to[2], 2));
}

OurVector<3> EquationSolver::getJacobianRow(OurVector<3>& coordinate, uint8_t tower_i, uint8_t tower_j)
{
    OurVector<3> jacobian_row;
    auto numerator = [](float tower_coordinate, float plane_coordinate) { return tower_coordinate - plane_coordinate; };
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
        jacobian_row[column] = numerator(coordinate[column], _towers_coordinates[tower_i][column]) / denumerator_i +
            numerator(coordinate[column], _towers_coordinates[tower_j][column]) / denumerator_j;
    }
    return jacobian_row;
}
