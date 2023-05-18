#include <matrix.h>
#include <extended_evaluation.h>

#include <utility>


static const double array_dispersion[] = { 1e4, 4356, 2, 1e4, 4356, 2, 1e4, 4356, 0.2 };
//static const double array_dispersion[] = { 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6 };
//static const double array_dispersion[] = { 1e4, 9000, 1e4, 1e4, 9000, 1e4, 1e5, 1e4, 1e4 };



ExtendedEvaluation::ExtendedEvaluation()
{
    _filter.setStateCovarianceMatrix(getCovarianceStateMatrix());

    OurMatrix<9, 9> covariance_error;
    _filter.setErrorCovarianceMatrix(covariance_error);

    OurMatrix<EQUATIONS_COUNT, EQUATIONS_COUNT> covariance_noise;
    //covariance_noise.setDiagonalValue(array_dispersion[4]);
    covariance_noise.setDiagonalValue(1e-6);
    _filter.setNoiseCovarianceMatrix(covariance_noise);

    //OurVector<3> pos;
    //pos.setValue(1);
    //updateObservationMatrix(pos);
    setObservationFunction();
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

OurVector<9> ExtendedEvaluation::estimatedState(OurVector<EQUATIONS_COUNT>& tdoa)
{
    _initial_tdoas = tdoa;
    _filter.predict(_time_delta);
    
    auto system = _filter.getSystemVector();
    OurVector<3> pos;
    pos[0] = system[0];
    pos[1] = system[3];
    pos[2] = system[6];
    updateObservationMatrix(pos);
    
    return _filter.correct(_initial_tdoas);
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
        jacobian_row[column * 3] = (numerator(coordinate[column], _towers_coordinates[tower_i][column]) / denominator_i -
                                    numerator(coordinate[column], _towers_coordinates[tower_j][column]) / denominator_j);
    }

    return jacobian_row;
}

OurMatrix<EQUATIONS_COUNT, 9> ExtendedEvaluation::getJacobian(OurVector<3> &position)
{
    auto eequation = [=](uint8_t i, uint8_t j, double x, double y, double z)
    {
        return (1 / LIGHT_SPEED) *
            std::sqrt(
                std::pow(_towers_coordinates[i][0] - x, 2) +
                std::pow(_towers_coordinates[i][1] - y, 2) +
                std::pow(_towers_coordinates[i][2] - z, 2)
            )
            -
            std::sqrt(
                std::pow(_towers_coordinates[j][0] - x, 2) +
                std::pow(_towers_coordinates[j][1] - y, 2) +
                std::pow(_towers_coordinates[j][2] - z, 2)
            );
    };

    auto Aobservation_func = [=](const OurVector<9>& coordinates)
    {
        uint8_t k = 0;
        OurVector<EQUATIONS_COUNT> tdoas;
        for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
        {
            for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
            {
                std::cout << coordinates[0] << " " << coordinates[3] << " " << coordinates[6] << std::endl;
                tdoas[k++] = eequation(i, j, coordinates[0], coordinates[3], coordinates[6]);
               // std::cout << tdoas[k - 1];
            }
        }
        std::cout << std::endl;
        return tdoas;
    };

    OurVector<9> _pseudo_state;
    _pseudo_state[0] = position[0];
    _pseudo_state[3] = position[1];
    _pseudo_state[6] = position[2];
    auto calculated_tdoa = Aobservation_func(_pseudo_state);
    std::cout << "calculated_tdoa" << std::endl;
    std::cout << calculated_tdoa << std::endl;
    std::cout << "init tdoa" << std::endl;
    std::cout << _initial_tdoas << std::endl << std::endl;
    
  //  std::cout << _pseudo_state << std::endl;
    OurMatrix<EQUATIONS_COUNT, 9> jacobian;
    //jacobian.setValue(0.1);
    uint8_t k = 0;
    for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
    {
        for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
        {
            jacobian[k++] = getJacobianRow(position, i, j);
        }
    }

    for (int iteration = 0; iteration < MAX_ITERARATIONS_COUNT; ++iteration)
    {
        k = 0;
        for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
        {
            for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
            {
                if (calculated_tdoa[k] < 0)
                {
                    jacobian[k] = -jacobian[k];
                    //_initial_tdoas[k] = -_initial_tdoas[k];
                }
                k++;
            }
        }
    }
    for (int i = 0; i < EQUATIONS_COUNT; ++i)
    {
        _initial_tdoas[i] = std::abs(_initial_tdoas[i]);
     //   std::cout << _initial_tdoas[i] << std::endl;
    }
    return (1 / LIGHT_SPEED) * jacobian;
}

void ExtendedEvaluation::updateObservationMatrix(OurVector<3>& position)
{
    auto H = getJacobian(position);
   // std::cout << H << "\n\n";

    _filter.setObservationMatrix(H);
}

void ExtendedEvaluation::setObservationFunction()
{
    auto equation = [=](uint8_t i, uint8_t j, double x, double y, double z)
    {
        return (1/LIGHT_SPEED) * std::abs(
                std::sqrt(
                       std::pow(_towers_coordinates[i][0] - x, 2) +
                       std::pow(_towers_coordinates[i][1] - y, 2) +
                       std::pow(_towers_coordinates[i][2] - z, 2))
                -
                std::sqrt(
                        std::pow(_towers_coordinates[j][0] - x, 2) +
                        std::pow(_towers_coordinates[j][1] - y, 2) +
                        std::pow(_towers_coordinates[j][2] - z, 2))
        );
    };

    auto observation_func = [=](const OurVector<9>& coordinates)
    {
        uint8_t k = 0;
        OurVector<EQUATIONS_COUNT> tdoas;
        for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
        {
            for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
            {
                tdoas[k++] = equation(i, j, coordinates[0], coordinates[3], coordinates[6]);
            }
        }

        return tdoas;
    };

    _filter.setFunction(observation_func);
}
