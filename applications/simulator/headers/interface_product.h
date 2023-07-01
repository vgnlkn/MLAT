#ifndef MLAT_INTERFACE_PRODUCT_H
#define MLAT_INTERFACE_PRODUCT_H

class IProduct
{
public:
    virtual ~IProduct() {}
    virtual void addPoint(double x, double y, double z) = 0;
};

#endif
