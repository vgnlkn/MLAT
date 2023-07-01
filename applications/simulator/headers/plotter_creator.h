#ifndef MLAT_PLOTTER_CREATOR_H
#define MLAT_PLOTTER_CREATOR_H

#include <interface_creator.h>
#include <plotter.h>

class PlotterCreator final : public ICreator
{
public:
    [[nodiscard]] IProduct* FactoryMethod(const std::string& file_name) const final { return new Plotter(file_name); }
};

#endif //MLAT_PLOTTER_CREATOR_H
