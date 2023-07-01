#include <processor.h>
#include <plotter.h>


Processor::Processor() : _plt_mlat(nullptr),
                         _plt_filter(nullptr),
                         _plt_filter_speed(nullptr),
                         _plt_filter_acceleration(nullptr),
                         _plt_standard_filter_speed(nullptr),
                         _plt_standard_filter_acceleration(nullptr),
                         _plt_standard_filter(nullptr),
                         _noise(new NoizeGenerator),
                         _iteration(1),
                         _overstatement(0) {}

void Processor::initSolver()
{
    OurVector<k_space_dim> init;
    OurVector<k_equations_count> tdoas;
    calculateTDOA(tdoas);

    _solver.setInitialParams(init, tdoas);

    OurVector<9> init_state;
    init_state[0] = init[0];
    init_state[3] = init[1];
    init_state[6] = init[2];

    _unscented_filter.setInitialParams(init_state);
}

void Processor::process(uint32_t iter)
{
    OurVector<k_equations_count> tdoas;
    calculateTDOA(tdoas);

    OurVector<k_space_dim> mlat_coords = _solver.solve(tdoas);
    OurVector<9> aircraft_trajectory_estimation = _unscented_filter.solve(tdoas);
    OurVector<9> standard_filter_estim = _estim.estimatedState(mlat_coords);

    auto fillVector = [](const OurVector<9>& estimation, OurVector<k_space_dim>& vector, uint8_t i) -> void
    {
        vector[0] = estimation[i];
        vector[1] = estimation[i + 3];
        vector[2] = estimation[i + 6];
    };


    auto addPoint = [](const OurVector<k_space_dim>& coords, Plotter* plt) -> void
    {
        if (plt)
        {
            if (coords[0] == coords[0] && coords[1] == coords[1]
                && coords[2] == coords[2])
            {
                plt->addPoint(coords[0], coords[1], coords[2]);
            }
        }
    };

    OurVector<k_space_dim> filter_coords, filter_speed, filter_acceleration;
    fillVector(aircraft_trajectory_estimation, filter_coords, 0);
    fillVector(aircraft_trajectory_estimation, filter_speed, 1);
    fillVector(aircraft_trajectory_estimation, filter_acceleration, 2);

    OurVector<k_space_dim> standard_filter_coords, standard_filter_speed, standard_filter_acceleration;
    fillVector(standard_filter_estim, standard_filter_coords, 0);
    fillVector(standard_filter_estim, standard_filter_speed, 1);
    fillVector(standard_filter_estim, standard_filter_acceleration, 2);

    _mlat_average = mlat_coords + _mlat_average;
    _kalman_average = filter_coords + _kalman_average;
    if (_iteration % 100 == 0)
    {
        _overstatement = 0;
        _mlat_average.setValue(0);
        _mlat_min = mlat_coords;
        _mlat_max = mlat_coords;
        _kalman_average.setValue(0);
        _iteration = 1;
    }

    for (uint8_t i = 0; i < k_space_dim; ++i)
    {
        if (mlat_coords[i] < _mlat_min[i])
        {
            _mlat_min[i] = mlat_coords[i];
        }
        else if (mlat_coords[i] > _mlat_max[i])
        {
            _mlat_max[i] = mlat_coords[i];
        }
        if (std::abs(_mlat_average[i] - _kalman_average[i]) > _iteration++ * std::abs(_mlat_min[i] - _mlat_max[i]))
        {
            _overstatement++;
        }
    }

    if (_overstatement > k_duration_overstatement)
    {
        _unscented_filter.reset();
        _estim.reset();
    }


    if (iter % k_point_mod == 0)
    {
        addPoint(mlat_coords, _plt_mlat);
        addPoint(filter_coords, _plt_filter);
        addPoint(filter_speed, _plt_filter_speed);
        addPoint(filter_acceleration, _plt_filter_acceleration);
        addPoint(standard_filter_acceleration, _plt_standard_filter_acceleration);
        addPoint(standard_filter_speed, _plt_standard_filter_speed);
        addPoint(standard_filter_coords, _plt_standard_filter);
    }
}

void Processor::setTower(uint16_t id, const Tower& tower)
{
    _towers[id] = tower;
    _towers_coordinates[id] = tower.getPosition();
    _solver.setTowersCoordinates(_towers_coordinates);
    _unscented_filter.setTowersCoordinates(_towers_coordinates);
}

void Processor::calculateTDOA(OurVector<k_equations_count>& tdoas)
{
    auto noize = [&](uint8_t i) -> double
    {
        return _towers_toa[i] * _noise->generate();
    };
    uint8_t k = 0;
    for (uint8_t i = 0; i < k_towers_count; ++i)
    {
        for (uint8_t j = i + 1; j < k_towers_count; ++j)
        {
            tdoas[k++] = _towers_toa[i] - _towers_toa[j] + noize(j);
        }
    }
}
