#include <processor.h>


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
    addPoint(coords);
}

void Processor::setTower(uint16_t id, const Tower& tower)
{
    _towers[id] = tower;
    _towers_coordinates[id] = tower.getPosition();
    _solver.setTowersCoordinates(_towers_coordinates);
}

void Processor::calculateTDOA(OurVector<EQUATIONS_COUNT>& tdoas)
{
    uint16_t k = 0;
    for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
    {
        for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
        {
            tdoas[k++] = _towers_toa[i] - _towers_toa[j] + addNoise();
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

double Processor::addNoise() const
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-0.00000001, 0.00000001);

    double randomNum = dis(gen);

    return randomNum;
}
