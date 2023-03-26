#ifndef EQUATION_SOLVER_H
#define EQUATION_SOLVER_H

#include <vector.h>
//#include <array>
#include <matrix.h>
#include <map>

class EquationSolver 
{
public:
    EquationSolver() = default;
    ~EquationSolver() = default;


    OurMatrix<EQUATIONS_COUNT, 3> getJacobian(OurVector<3>& position);
    void setTowersCoordinates(std::map<uint16_t, OurVector<3>> tower_coordinates);
    void setInitialParams(const OurVector<3>& initial_coordinates, const OurVector<EQUATIONS_COUNT>& initial_tdoas);

    OurVector<3> solve(OurVector<EQUATIONS_COUNT>& tdoas);
private:
    float distance(const OurVector<3> from, const OurVector<3> to);
    OurVector<3> getJacobianRow(OurVector<3>& position, uint8_t tower_i, uint8_t tower_j);

private:
    OurVector<3> _initial_coordinates;
    OurVector<EQUATIONS_COUNT> _initial_tdoas;
    std::map<uint16_t, OurVector<3>> _towers_coordinates;
};

#endif