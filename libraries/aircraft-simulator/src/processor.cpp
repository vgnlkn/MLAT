#include <processor.h>

void Processor::initSolver()
{
    OurVector<3> init;
    OurVector<EQUATIONS_COUNT> tdoas;
    calculateTDOA(tdoas);

    _solver.setInitialParams(init, tdoas);
}

void Processor::addTOA(uint16_t id, double TOA)
{
    _towers_toa[id] = TOA;
}

int k = 0;
void Processor::process(uint32_t iter)
{
    OurVector<EQUATIONS_COUNT> tdoas;
    calculateTDOA(tdoas);

    //std::cout << tdoas << std::endl;
    OurVector<3> mlat_coords = _solver.solve(tdoas);
    
    //OurVector<9> aircraft_trajectory_estimation = _estim.estimatedState(mlat_coords);
    // std::cout << tdoas << '\n';
    //_eval.updateObservationMatrix(mlat_coords);
  //  tdoas = _eval.getTDOA();
    //if (k <100)
    //{
    // //   k = false;
    //    OurVector<9> st;
    //    st[0] = mlat_coords[0];
    //    st[3] = mlat_coords[1];
    //    st[6] = mlat_coords[2];
    //    _eval.initState(st);
    //    k++;
    //}
    //std::cout << tdoas << std::endl;
    OurVector<9> aircraft_trajectory_estimation = _eval.estimatedState(tdoas);

    // std::cout << aircraft_trajectory_estimation << '\n';

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


    _mlat_average = mlat_coords + _mlat_average;
    _kalman_average = filter_coords + _kalman_average;
    if (_iteration % 100 == 0)
    {
        _overstatement = 0;
        _mlat_average.setValue(0);
        _mlat_min = mlat_coords;
        _mlat_max = mlat_coords;
        _kalman_average.setValue(0);
        _iteration = 1;
    }
   
    for (int i = 0; i < 3; ++i)
    {
        if (mlat_coords[i] < _mlat_min[i])
        {
            _mlat_min[i] = mlat_coords[i];
        }
        else if (mlat_coords[i] > _mlat_max[i])
        {
            _mlat_max[i] = mlat_coords[i];
        }
        if (std::abs(_mlat_average[i] - _kalman_average[i]) > _iteration++ * std::abs(_mlat_min[i] - _mlat_max[i]))
        {
            _overstatement++;
        }
    }

    if (_overstatement > k_duration_overstatement)
    {
        aircraft_trajectory_estimation[8] *= -1;
        aircraft_trajectory_estimation[5] *= -1;
        aircraft_trajectory_estimation[2] *= -1;
        
        /*aircraft_trajectory_estimation[1] = 0;
        aircraft_trajectory_estimation[4] = 0;
        aircraft_trajectory_estimation[7] = 0;*/

         //_estim.initState(aircraft_trajectory_estimation);
       // _eval.initState(aircraft_trajectory_estimation);
        // _estim.reset();
    //    _eval.reset();
    }

    if (iter % POINT_MOD == 0)
   // if (1==1)
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
    _eval.setTowersCoordinates(_towers_coordinates);
}

void Processor::setSampleRate(double sample_rate)
{
    //_estim.updateStateMatrix(sample_rate);
    _eval.updateStateMatrix(sample_rate);
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
            tdoas[k++] = _towers_toa[i] - _towers_toa[j] +noize(j);
        }
    }
}