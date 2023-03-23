#ifndef MLAT_PROCESSOR_H
#define MLAT_PROCESSOR_H

#include <chrono>
#include <vector>
#include <vector.h>

class Processor
{
public:
    Processor() = default;

    //! Accept new time
    void acceptSignal(float time) { _times.push_back(time); }
    //! Accept new coordinates
    void acceptCoordinates(const OurVector<3>& coordinates) { _coordinates.push_back(coordinates); }

    //! Getters
    std::vector<double> getTimes() { return _times; }
    std::vector<OurVector<3>> getCoordinates() { return _coordinates; }

    void start();

    //! Print result of aircraft flight
    void printResult();

private:
    std::vector<double> _times;
    std::vector<OurVector<3>> _coordinates;
};

#endif 
