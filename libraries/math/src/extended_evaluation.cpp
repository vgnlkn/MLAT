#include <matrix.h>
#include <extended_evaluation.h>


static const double array_dispersion[] = {1e4, 9000, 10, 1e4, 9000, 10, 1e4, 1e3, 10};

ExtendedEvaluation::ExtendedEvaluation()
{
    _filter.setStateCovarianceMatrix(getCovarianceStateMatrix());

    OurMatrix<9, 9> covariance_error;
    _filter.setErrorCovarianceMatrix(covariance_error);

    OurMatrix<EQUATIONS_COUNT, EQUATIONS_COUNT> covariance_noise;
    covariance_noise.setDiagonalValue(array_dispersion[4]);
    _filter.setNoiseCovarianceMatrix(covariance_noise);

    OurMatrix<EQUATIONS_COUNT, 9> observation_matrix;
    observation_matrix.setZero();
    observation_matrix[0][0] = 1;
    observation_matrix[1][3] = 1;
    observation_matrix[2][6] = 1;
    _filter.setObservationMatrix(observation_matrix);
}


void ExtendedEvaluation::updateStateMatrix(double time_delta)
{
    OurMatrix<9, 9> state_matrix;
    state_matrix.setIdentity();
    for (int i = 0; i < 9; i += 3)
    {
        state_matrix[i][i + 1] = time_delta;
        state_matrix[i][i + 2] = time_delta * time_delta * 0.5;
        state_matrix[i + 1][i + 2] = time_delta;
    }
    _time_delta = time_delta;
    _filter.setStateMatrix(state_matrix);
}

void ExtendedEvaluation::initState(OurVector<9>& initial_state)
{
    _filter.setSystemVector(initial_state);
}

OurVector<9> ExtendedEvaluation::estimatedState(OurVector<EQUATIONS_COUNT>& observation)
{
    _filter.predict(_time_delta);
    return _filter.correct(observation);
}

OurMatrix<9, 9> ExtendedEvaluation::getCovarianceStateMatrix()
{
    OurMatrix<9, 9> covariance_state;
    for (uint8_t i = 0; i < 9; ++i)
    {
        covariance_state[i][i] = array_dispersion[i];
    }

    return covariance_state;
}


OurVector<9> ExtendedEvaluation::getJacobianRow(OurVector<3> &coordinate, uint8_t tower_i, uint8_t tower_j)
{
    OurVector<9> jacobian_row;
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

OurMatrix<EQUATIONS_COUNT, 9> ExtendedEvaluation::getJacobian(OurVector<3> &position) {
    OurMatrix<EQUATIONS_COUNT, 9> jacobian;
    uint8_t k = 0;
    for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
    {
        for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
        {
            jacobian[k] = getJacobianRow(position, i, j);
            k += 3;
        }
    }

    return (1 / LIGHT_SPEED) * jacobian;
}
