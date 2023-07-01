#ifndef MLAT_PLOTTER_CREATOR_H
#define MLAT_PLOTTER_CREATOR_H

#include <interface_creator.h>
#include <plotter.h>

/**
 * \class PlotterCreator
 * \brief PlotterCreator class inherited from class ICreator and creates Plotter object
 **/
class PlotterCreator final : public ICreator
{
public:
    //! Factory Method, that creates and return plotter object
    [[nodiscard]] IProduct* FactoryMethod(const std::string& file_name) const final { return new Plotter(file_name); }
};

#endif //MLAT_PLOTTER_CREATOR_H
