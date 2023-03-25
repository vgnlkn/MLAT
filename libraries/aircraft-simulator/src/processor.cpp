#include <processor.h>

void Processor::addTOA(uint16_t id, const std::set<float> &TDOA)
{
    _map[id] = TDOA;
}

void Processor::calculateTDOA()
{
}