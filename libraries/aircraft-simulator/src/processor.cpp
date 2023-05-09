#include <processor.h>
#include <iostream>

static const uint32_t k_duration_interval = 100;
static const uint32_t k_duration_overstatement = 0.05 * k_duration_interval;

void Processor::initSolver()
{
    OurVector<3> init;
    OurVector<EQUATIONS_COUNT> tdoas;
    calculateTDOA(tdoas);

    _solver.setInitialParams(init, tdoas);
}

void Processor::addTOA(uint16_t id, double TOA)
{
    _towers_toa[id] = TOA;
}

void Processor::process()
{
    auto addPoint = [](const OurVector<3>& coords, Plotter* plt)
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

    OurVector<EQUATIONS_COUNT> tdoas;
    calculateTDOA(tdoas);

    OurVector<3> coords = _solver.solve(tdoas);
    OurVector<9> aircraft_trajectory_estimation = 
        _estim.estimatedState(coords);

    auto fillVector = [=](OurVector<3>& vector, uint8_t i)
    {
        vector[0] = aircraft_trajectory_estimation[i];
        vector[1] = aircraft_trajectory_estimation[i + 3];
        vector[2] = aircraft_trajectory_estimation[i + 6];
    };

    OurVector<3> filter_coords, filter_speed;
    fillVector(filter_coords, 0);
    fillVector(filter_speed, 1);


    _mlat_average = coords + _mlat_average;
    _kalman_average = filter_coords + _kalman_average;
    std::cout << aircraft_trajectory_estimation[8] << std::endl;
    if (_iteration % 100 == 0)
    {
        _overstatement = 0;
        _mlat_average.setValue(0);
        _mlat_min = coords;
        _mlat_max = coords;
        _kalman_average.setValue(0);
        _iteration = 1;
    }
   
    for (int i = 0; i < 3; ++i)
    {
        if (coords[i] < _mlat_min[i])
        {
            _mlat_min[i] = coords[i];
        }
        else if (coords[i] > _mlat_max[i])
        {
            _mlat_max[i] = coords[i];
        }
        if (std::abs(_mlat_average[i] - _kalman_average[i]) > _iteration++ * std::abs(_mlat_min[i] - _mlat_max[i]))
        {
            _overstatement++;
        }
    }

    if (_overstatement > k_duration_overstatement)
    {
        std::cout << _overstatement << ") Restart filter." << std::endl;
        aircraft_trajectory_estimation[8] = 0;
        aircraft_trajectory_estimation[5] = 0;
        aircraft_trajectory_estimation[2] = 0;

        _estim.initState(aircraft_trajectory_estimation);
        _estim.reset();
    }

    addPoint(coords, _plt_mlat);
    addPoint(filter_coords, _plt_filter);
    addPoint(filter_speed, _plt_filter_speed);
}

void Processor::setTower(uint16_t id, const Tower& tower)
{
    _towers[id] = tower;
    _towers_coordinates[id] = tower.getPosition();
    _solver.setTowersCoordinates(_towers_coordinates);
}

void Processor::setSampleRate(double sample_rate)
{
    _estim.updateStateMatrix(sample_rate);
}

void Processor::calculateTDOA(OurVector<EQUATIONS_COUNT>& tdoas)
{
    auto noize = [=](int i) -> double
    {
        return _towers_toa[i] * _noise->generate();
    };
    uint16_t k = 0;
    for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
    {
        for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
        {
            tdoas[k++] = _towers_toa[i] + noize(i) - _towers_toa[j] + noize(j);
        }
    }
}