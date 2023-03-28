#include <processor.h>


Processor::Processor()
{
    OurVector<3> init;
    OurVector<6> init_tdoas;
    init_tdoas[0] = 0.00729437;
    init_tdoas[1] = 0.0381568;
    init_tdoas[2] = 0.0338469;
    init_tdoas[3] = 0.0308624;
    init_tdoas[4] = 0.0265525;
    init_tdoas[5] = 0.00430991;
    _solver.setInitialParams(init, init_tdoas);
}

void Processor::addTOA(uint16_t id, const std::stack<float> &TOA)
{
    _towers_toa[id] = TOA;
}

void Processor::process()
{
    OurVector<EQUATIONS_COUNT> tdoas;
    uint16_t k = 0;
    for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
    {
        for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
        {
            tdoas[k++] = _towers_toa[i].top() - _towers_toa[j].top();
        }
    }

    auto coords = _solver.solve(tdoas);
    if (_plt)
    {
        if (coords[0] == coords[0] && coords[1] == coords[1]
        && coords[2] == coords[2])
        {
            _plt->addPoint(coords[0], coords[1], coords[2]);
        }
    }
}

void Processor::setTower(uint16_t id, const Tower& tower)
{
    _towers[id] = tower;
    _towers_coordinates[id] = tower.getPosition();
    _solver.setTowersCoordinates(_towers_coordinates);
}