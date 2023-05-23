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

    void setInitialParams(const OurVector<3>& initial_coordinates,
        const OurVector<EQUATIONS_COUNT>& initial_tdoas);


    //! Update state matrix
    void updateStateMatrix(double time_delta);
    //! Update observation matrix
    void updateObservationMatrix(OurVector<3>& position);
    //! Initial state for filter
    // void initState(OurVector<9>& initial_state) { _filter.setSystemVector(initial_state); };
    //! Get default state covariance state matrix
    OurMatrix<3, 3> getCovarianceStateMatrix();
    //! Estimated state
    OurVector<3> estimatedState(OurVector<EQUATIONS_COUNT>& tdoas);
    //! Resetes covariance matrixes;
    // void reset() { _filter.setStateCovarianceMatrix(getCovarianceStateMatrix()); }

    //! Returns Jacobian for matrix H
    OurMatrix<EQUATIONS_COUNT, 3> getJacobian(OurVector<3> &position);
    //! Returns Jacobian matrix row
    OurVector<3> getJacobianRow(OurVector<3>& coordinate, uint8_t tower_i, uint8_t tower_j);
    //!
    void setTowersCoordinates(std::map<uint16_t, OurVector<3>> tower_coordinates) { _towers_coordinates =
                                                                                    std::move(tower_coordinates); }
    //!
    void setObservationFunction();
private:
    //! Kalman Filter
    ExtendedFilter<3, EQUATIONS_COUNT> _filter;
    //! Sample rate
    double _time_delta;
    //! TDOA
    OurVector<EQUATIONS_COUNT> _initial_tdoas;
    //! Tower coordinates
    std::map<uint16_t, OurVector<3>> _towers_coordinates;
};


#endif //MLAT_EXTENDED_EVALUATION_H
