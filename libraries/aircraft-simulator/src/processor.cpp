#include <processor.h>
#include <iostream>

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
    OurVector<EQUATIONS_COUNT> tdoas;
    uint16_t k = 0;
    calculateTDOA(tdoas);

    OurVector<3> coords = _solver.solve(tdoas);
    OurVector<9> aircraft_trajectory_estimation = 
        _estim.estimatedState(coords);

    coords[0] = aircraft_trajectory_estimation[0];
    coords[1] = aircraft_trajectory_estimation[3];
    coords[2] = aircraft_trajectory_estimation[6];
    std::cout << coords << std::endl;
    addPoint(coords);
    
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

void Processor::addPoint(const OurVector<3>& coords)
{
    if (_plt)
    {
        if (coords[0] == coords[0] && coords[1] == coords[1]
            && coords[2] == coords[2])
        {
            _plt->addPoint(coords[0], coords[1], coords[2]);
        }
    }
}
