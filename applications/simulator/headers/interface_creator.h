#ifndef MLAT_INTERFACE_CREATOR_H
#define MLAT_INTERFACE_CREATOR_H

#include <string>

class IProduct;

/**
 * \class ICreator
 * \brief ICreator class declares a factory method that must return an object
 * class Product. Creator subclasses usually provide an implementation of this
 * method.
 **/
class ICreator
{
public:
    //! Virtual destructor
    virtual ~ICreator() = default;
    //! Factory method, that creates and returns product
    [[nodiscard]] virtual IProduct* FactoryMethod(const std::string& file_name) const = 0;
};

#endif
