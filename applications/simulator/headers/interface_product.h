#ifndef MLAT_INTERFACE_PRODUCT_H
#define MLAT_INTERFACE_PRODUCT_H

/**
 * \class IProduct
 * \brief IProduct class represents all operations that all products must do
 **/
class IProduct
{
public:
    //! Destructor
    virtual ~IProduct() = default;
    //! Add point for plotter
    virtual void addPoint(double x, double y, double z) = 0;
};

#endif
