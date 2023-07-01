#ifndef MLAT_INTERFACE_CREATOR_H
#define MLAT_INTERFACE_CREATOR_H

#include <string>

class IProduct;

class ICreator
{
public:
    virtual ~ICreator() {}
    [[nodiscard]] virtual IProduct* FactoryMethod(const std::string& file_name) const = 0;
};

#endif
