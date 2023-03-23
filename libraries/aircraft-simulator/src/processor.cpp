#include <field.h>
#include <processor.h>

void Processor::printResult()
{
    for (uint16_t i = 0; i < 1000; ++i)
    {
        std::cout << "current _aircraft position: " << _coordinates[i] << " with time: " << _times[i] << '\n';
    }
}

void Processor::start()
{
    OurVector<3> a;
    a.setValue(1);

    Aircraft aircraft(a);
    aircraft.calculateNewSpeed();
    Field obj(aircraft);
    obj.startMovement();
}
