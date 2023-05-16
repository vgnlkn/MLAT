#ifndef MLAT_EXTENDED_EVALUATION_H
#define MLAT_EXTENDED_EVALUATION_H

#include <extended_filter.h>
#include <map>
#include <utility>


class ExtendedEvaluation
{
public:
    //! Constructor
    ExtendedEvaluation();
    //! Destructor
    ~ExtendedEvaluation() = default;
    //! Update state matrix
    void updateStateMatrix(double time_delta);
    //! Update observation matrix
    void updateObservationMatrix(OurVector<3>& position);
    //! Initial state for filter
    void initState(OurVector<9>& initial_state) { _filter.setSystemVector(initial_state); };
    //! Get default state covariance state matrix
    OurMatrix<9, 9> getCovarianceStateMatrix();
    //! Estimated state
    OurVector<9> estimatedState(OurVector<EQUATIONS_COUNT>& tdoas);
    //! Resetes covariance matrixes;
    void reset() { _filter.setStateCovarianceMatrix(getCovarianceStateMatrix()); }

    //!
    OurMatrix<EQUATIONS_COUNT, 9> getJacobian(OurVector<3> &position);
    //!
    OurVector<9> getJacobianRow(OurVector<3>& coordinate, uint8_t tower_i, uint8_t tower_j);
    //!
    void setTowersCoordinates(std::map<uint16_t, OurVector<3>> tower_coordinates) { _towers_coordinates =
                                                                                    std::move(tower_coordinates); }
    //!
    void setObservationFunction();

    //!
    [[nodiscard]] OurVector<EQUATIONS_COUNT> getTDOA() const { return _initial_tdoas; }
private:
    //! Kalman Filter
    ExtendedFilter<9, EQUATIONS_COUNT> _filter;
    //! Sample rate
    double _time_delta;
    //! tdoa
    OurVector<EQUATIONS_COUNT> _initial_tdoas;
    //! Tower coordinates
    std::map<uint16_t, OurVector<3>> _towers_coordinates;
};

#endif
