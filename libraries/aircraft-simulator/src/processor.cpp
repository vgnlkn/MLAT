#include <processor.h>

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
            tdoas[k++] = std::abs(_towers_toa[i].top() - _towers_toa[j].top());
        }
    }

    //std::cout << tdoas << std::endl;
    auto coords = _solver.solve(tdoas);
    if (_plt)
    {
        //  std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (coords[0] == coords[0]&& coords[1] == coords[1] && coords[2] == coords[2])
        {
            _plt->addPoint(coords[0], coords[1], coords[2]);
        }
    }

    //_TDOA.push_back(tdoas);
}