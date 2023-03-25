#ifndef MLAT_PROCESSOR_H
#define MLAT_PROCESSOR_H

#include <chrono>
#include <vector>
#include <vector.h>
#include <map>
#include <set>
#include <tower.h>


class Processor
{
public:
    Processor() = default;

    //! Calculate TDOA for one iteration
    void addTOA(uint16_t id, const std::set<float>& TDOA);
    //! Overloading operator[]
    std::set<float>& operator[](uint16_t id) { return _map[id]; }

    //! Calculate TDOA on the last iteration
    void calculateTDOA();
private:
    std::map<uint16_t, std::set<float>> _map;
    std::vector<OurVector<6>> _TDOA;
};

#endif 
