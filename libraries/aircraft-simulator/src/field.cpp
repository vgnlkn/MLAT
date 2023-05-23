#include <field.h>


Field::Field() :
        _towers(new Tower[TOWERS_COUNT]),
        _plt_flight(nullptr),
        _plt_speed(nullptr),
        _plt_acceleration(nullptr),
        _tower_count(TOWERS_COUNT),
        _sample_rate(k_sample_rate)
{
    _processor.setSampleRate(_sample_rate);
    _aircraft.setTimeDelta(_sample_rate);
}

void Field::startMovement()
{
    initialize();

    for (uint32_t i = 0;; ++i)
    {
        updateAircraftPosition();
        checkHeight();
        updateAircraftSpeed();
        sendSignalsToTowers();
        _processor.getEval().updateObservationMatrix(_current_position);
        processSignals(i);
        if (i % POINT_MOD == 0)
        {
            updatePlot();
        }
    }
}

void Field::initialize()
{
    setTowers();
    setAircraftTowers();
    sendSignalsToTowers();
    _processor.initSolver();
    _aircraft.checkAcceleration();

    //_processor.getEval().updateObservationMatrix(_current_position);
}

void Field::updateAircraftPosition()
{
    _current_position = _current_position +
                        _aircraft.getSpeed() * _sample_rate;
}

void Field::updateAircraftSpeed()
{
    _aircraft.calculateNewSpeed();
    _aircraft.checkSpeed();
}

void Field::sendSignalsToTowers()
{
    double toa;
    for (uint16_t j = 0; j < _tower_count; ++j)
    {
        Tower& tower = _towers[j];
        toa = _aircraft.sendSignal(tower, _current_position);
        _processor.addTOA(tower.getID(), toa);
        _processor.setTower(tower.getID(), tower);
    }
}

void Field::updatePlot()
{
    auto drawPoint = [](Plotter* plt, const OurVector<3>& param)
    {
        if (plt)
        {
            //std::this_thread::sleep_for(std::chrono::milliseconds(10));
            plt->addPoint(param[0], param[1], param[2]);
        }
    };

    drawPoint(_plt_flight, _current_position);
    drawPoint(_plt_speed, _aircraft.getSpeed());
    drawPoint(_plt_acceleration, _aircraft.getAcceleration());
}

void Field::checkHeight()
{
    if (_current_position[2] > 10.f && _aircraft.getSpeed()[2] > 0.f)
    {
        decreaseVerticalSpeed();
    }
    if (_aircraft.getSpeed()[2] < -0.1f)
    {
        stopVerticalSpeed();
    }
}

void Field::decreaseVerticalSpeed()
{
    OurVector<3> new_acceleration = _aircraft.getAcceleration();
    new_acceleration[2] = new_acceleration[2] > 1e-4 ? new_acceleration[2] / 1.5f : new_acceleration[2] - 1e-5;

    _aircraft.setAcceleration(new_acceleration);
    _aircraft.calculateNewSpeed();
}

void Field::stopVerticalSpeed()
{
    OurVector<3> new_speed = _aircraft.getSpeed();
    new_speed[2] = 0;

    OurVector<3> new_acceleration = _aircraft.getAcceleration();
    new_acceleration[2] = 0;

    _aircraft.setSpeed(new_speed);
    _aircraft.setAcceleration(new_acceleration);
}

void Field::setTowers()
{
    OurVector<3> tower_position;
    tower_position[0] = -1500;
    tower_position[1] = 400;
    tower_position[2] = 1200;

    _towers[0].setID(0);
    _towers[0].setPosition(tower_position);

    tower_position[0] = 2000;
    tower_position[1] = -3000;
    tower_position[2] = 0;

    _towers[1].setID(1);
    _towers[1].setPosition(tower_position);

    tower_position[0] = -5000;
    tower_position[1] = 12000;
    tower_position[2] = -900;

    _towers[2].setID(2);
    _towers[2].setPosition(tower_position);

    tower_position[0] = 11000;
    tower_position[1] = -4000;
    tower_position[2] = 2000;

    _towers[3].setID(3);
    _towers[3].setPosition(tower_position);

    tower_position[0] = 7500;
    tower_position[1] = 7000;
    tower_position[2] = 500;

    _towers[4].setID(4);
    _towers[4].setPosition(tower_position);
}

void Field::setAircraftTowers()
{
    for (uint16_t i = 0; i < _tower_count; ++i)
    {
        _aircraft.setNewTower(_towers[i]);
    }
}
