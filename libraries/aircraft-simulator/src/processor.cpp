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

    //_TDOA.push_back(tdoas);
}