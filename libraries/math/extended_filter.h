#ifndef MLAT_EXTENDED_FILTER_H
#define MLAT_EXTENDED_FILTER_H

#include <kalman_filter.h>
#include <defines.h>
#include <functional>

template<uint8_t dim_state, uint8_t dim_observation>
class ExtendedFilter final : public KalmanFilter<dim_state, dim_observation> {
public:
    ExtendedFilter() = default;

    virtual OurVector<dim_observation> getError(const OurVector<dim_observation> &state_vector) final;
    void setFunction(std::function<OurVector<dim_observation>(OurVector<dim_state>)> func) { _observation_function = func; }
private:
    std::function<OurVector<dim_observation>(OurVector<dim_state>)> _observation_function;
};

template<uint8_t dim_state, uint8_t dim_observation>
OurVector<dim_observation>
ExtendedFilter<dim_state, dim_observation>::getError(const OurVector<dim_observation> &state_vector)
{
    return _observation_function(this->_system_vector) - state_vector * LIGHT_SPEED;
}

#endif //MLAT_EXTENDED_FILTER_H