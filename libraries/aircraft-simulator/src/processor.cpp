#include <processor.h>

void Processor::initSolver()
{
    OurVector<3> init;
    OurVector<EQUATIONS_COUNT> tdoas;
    calculateTDOA(tdoas);

    _solver.setInitialParams(init, tdoas);

    OurVector<9> init_state;
    init_state[0] = init[0];
    init_state[3] = init[1];
    init_state[6] = init[2];

    _nkf.setInitialParams(init_state, tdoas);
    // _ekf.setInitialParams(init, tdoas);
    //_eval.setInitialParams(init, tdoas);
}

void Processor::addTOA(uint16_t id, double TOA)
{
    _towers_toa[id] = TOA;
}

void Processor::process(uint32_t iter)
{
    OurVector<EQUATIONS_COUNT> tdoas;
    OurVector<EQUATIONS_COUNT> tdoass;
    calculateTDOA(tdoas);

    OurVector<3> mlat_coords = _solver.solve(tdoas);
    OurVector<9> aircraft_trajectory_estimation =  _nkf.solve(tdoas);

    auto fillVector = [=](OurVector<3>& vector, uint8_t i) -> void
    {
        vector[0] = aircraft_trajectory_estimation[i];
        vector[1] = aircraft_trajectory_estimation[i + 3];
        vector[2] = aircraft_trajectory_estimation[i + 6];
    };


    auto addPoint = [](const OurVector<3>& coords, Plotter* plt) -> void
    {
        if (plt)
        {
            if (coords[0] == coords[0] && coords[1] == coords[1]
                && coords[2] == coords[2])
            {
                plt->addPoint(coords[0], coords[1], coords[2]);
            }
        }
    };

    OurVector<3> filter_coords, filter_speed, filter_acceleration;
    fillVector(filter_coords, 0);
    fillVector(filter_speed, 1);
    fillVector(filter_acceleration, 2);


    if (iter % POINT_MOD == 0)
    {
        addPoint(mlat_coords, _plt_mlat);
        addPoint(filter_coords, _plt_filter);
        addPoint(filter_speed, _plt_filter_speed);
        addPoint(filter_acceleration, _plt_filter_acceleration);
    }
}

void Processor::setTower(uint16_t id, const Tower& tower)
{
    _towers[id] = tower;
    _towers_coordinates[id] = tower.getPosition();
    _solver.setTowersCoordinates(_towers_coordinates);
    _nkf.setTowersCoordinates(_towers_coordinates);
    // _eval.setTowersCoordinates(_towers_coordinates);
    // _ekf.init(_towers_coordinates);
}

void Processor::setSampleRate(double sample_rate)
{
    //_eval.updateStateMatrix(sample_rate);
}

void Processor::calculateTDOA(OurVector<EQUATIONS_COUNT>& tdoas)
{
    auto noize = [=](int i) -> double
    {
        return _towers_toa[i] * _noise->generate();
    };
    uint16_t k = 0;
    for (uint8_t i = 0; i < TOWERS_COUNT; ++i)
    {
        for (uint8_t j = i + 1; j < TOWERS_COUNT; ++j)
        {
            tdoas[k++] = _towers_toa[i] - _towers_toa[j] + noize(j);
        }
    }
}