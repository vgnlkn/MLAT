#include <processor.h>

void Processor::addTOA(uint16_t id, const std::stack<float> &TOA)
{
    _map[id] = TOA;
}

void Processor::calculateTDOA()
{
    OurVector<6> vector;
    uint16_t k = 0;
    for (uint16_t i = 0; i < 3; ++i)
    {
        for (uint16_t j = i + 1; j < 4; ++j)
        {
            vector[k] = std::abs(_map[i].top() - _map[j].top());
            ++k;
        }
    }

    _TDOA.push_back(vector);
}