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

    //! Calculate TOA for one iteration
    void addTOA(uint16_t id, const std::stack<float>& TOA);
    //! Overloading operator[]
    std::stack<float>& operator[](uint16_t id) { return _map[id]; }

    //! Calculate TDOA on the last iteration
    void calculateTDOA();

    //! Getter for _TDOA;
    [[nodiscard]] std::vector<OurVector<6>> getTDOA() const { return _TDOA; }
private:
    std::map<uint16_t, std::stack<float>> _map;
    std::vector<OurVector<6>> _TDOA;
};

#endif 
