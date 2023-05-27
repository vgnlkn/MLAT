#include <matrix.h>
#include <extended_evaluation.h>

#include <utility>

//static const double array_dispersion[] = { 1e4, 4356, 2, 1e4, 4356, 2, 1e4, 4356, 0.2 };
//static const double array_dispersion[] = { 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6 };
//static const double array_dispersion[] = { 1e5, 9000, 1e4, 1e4, 9000, 1e4, 1e4, 1e4, 1e4 };
static const double array_dispersion[] = { 1,1,1,1,1,1,1,1,1,1,1,1,1 };


ExtendedEvaluation::ExtendedEvaluation()
{
    _filter.init(_towers_coordinates);
}

/*
void ExtendedEvaluation::setInitialParams(const OurVector<3>& initial_coordinates, const OurVector<EQUATIONS_COUNT>& initial_tdoas)
{
    auto pos = initial_coordinates;
    updateObservationMatrix(pos);
}
*/

/*
void ExtendedEvaluation::updateStateMatrix(double time_delta)
{
    OurMatrix<3, 3> state_matrix;
    state_matrix.setIdentity();

    // state_matrix[0][1] = time_delta;
    // state_matrix[0][2] = time_delta * time_delta * 0.5;
    // state_matrix[1][2] = time_delta;
    _time_delta = time_delta;
    _filter.setStateMatrix(state_matrix);
}
*/

OurVector<3> ExtendedEvaluation::estimatedState(OurVector<EQUATIONS_COUNT> tdoas)
{
    // return _filter.estimate(tdoas);
}

OurMatrix<3, 3> ExtendedEvaluation::getCovarianceStateMatrix()
{
    OurMatrix<3, 3> covariance_state;
    for (uint8_t i = 0; i < 3; ++i)
    {
        covariance_state[i][i] = array_dispersion[i];
    }

    return covariance_state;
}


OurVector<3> ExtendedEvaluation::getJacobianRow(OurVector<3>& coordinate, uint8_t tower_i, uint8_t tower_j)
{
    OurVector<3> jacobian_row;
    auto numerator = [](double tower_coordinate, double plane_coordinate) { return plane_coordinate - tower_coordinate; };
    auto denominator = [=](uint8_t index, double x, double y, double z)
    {
        return std::sqrt(
                std::pow(_towers_coordinates[index][0] - x, 2) +
                std::pow(_towers_coordinates[index][1] - y, 2) +
                std::pow(_towers_coordinates[index][2] - z, 2)
        );
    };

    double denominator_i = denominator(tower_i, coordinate[0], coordinate[1], coordinate[2]);
    double denominator_j = denominator(tower_j, coordinate[0], coordinate[1], coordinate[2]);
    assert(denominator_i && denominator_j);

    for (int column = 0; column < 3; ++column)
    {
        jacobian_row[column] = (numerator(coordinate[column], _towers_coordinates[tower_i][column]) / denominator_i -
                                numerator(coordinate[column], _towers_coordinates[tower_j][column]) / denominator_j);
    }
    return jacobian_row;
}


OurMatrix<EQUATIONS_COUNT, 3> ExtendedEvaluation::getJacobian(OurVector<3>& position)
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
   // std::cout << jacobian << std::endl << std::endl;
    return jacobian;
}

/*
void ExtendedEvaluation::updateObservationMatrix(OurVector<3>& position)
{
    auto H = getJacobian(position);
    _filter.setObservationMatrix(H);
}
 */

/*
void ExtendedEvaluation::setObservationFunction()
{
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
        return d_i - d_j;

    };

    auto observation_func = [=](const OurVector<3>& coordinates)
    {
        uint8_t k = 0;
        OurVector<EQUATIONS_COUNT> tdoas;
        for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
        {
            for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
            {
                tdoas[k] = equation(coordinates, i, j);
                k++;
            }
        }
       // std::cout << coordinates << std::endl;
        return tdoas;
    };

    _filter.setFunction(observation_func);
} */