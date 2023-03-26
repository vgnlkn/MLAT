#include <processor.h>

void Processor::addTOA(uint16_t id, const std::stack<float> &TOA)
{
    _towers_toa[id] = TOA;
}

void Processor::process()
{
    OurVector<EQUATIONS_COUNT> tdoas;
    uint16_t k = 0;
    for (uint16_t i = 0; i < TOWERS_COUNT; ++i)
    {
        for (uint16_t j = i + 1; j < TOWERS_COUNT - 1; ++j)
        {
            tdoas[k++] = std::abs(_towers_toa[i].top() - _towers_toa[j].top());
        }
    }
    OurVector<3> pos;
    pos[0] = 100;
    pos[1] = 100;
    pos[2] = 100;
    OurMatrix<EQUATIONS_COUNT, 3> jac = _solver.getJacobian(pos);
    //_TDOA.push_back(tdoas);
}

void Processor::setTower(uint16_t id, const Tower& tower)
{
    _towers[id] = tower;
    _towers_coordinates[id] = tower.getPosition();
    _solver.setTowersCoordinates(_towers_coordinates);
}