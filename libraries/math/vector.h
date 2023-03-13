#ifndef MLAT_VECTOR_H
#define MLAT_VECTOR_H

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>


template<uint8_t size, typename type = float>
class OurVector
{
public:
    //!Default constructor
    OurVector() : vector(new type[size]) {}
    //! Copy constructor
    OurVector(const OurVector& other);
    //! Overloading operator -
    OurVector<size, type>& operator=(const OurVector& other);
    //! Destructor
    ~OurVector() { delete[] vector; }

    //! Overloading operator[] with link
    type& operator[](uint8_t i) { return vector[i]; }

    //! Overloading sum with another vector
    OurVector<size, type> operator+(const OurVector& other);
    //! Overloading subtraction with another vector
    OurVector<size, type> operator-(const OurVector& other);
    //! Overloading multiplying with another vector
    OurVector<size, type> operator*(const OurVector& other);

    //! Get size
    [[nodiscard]] uint8_t inline getSize() const { return size; }
    //! Check size
    void checkSize(const OurVector& other) { assert(other.getSize() == size); }
private:
    type* vector;
};

//! Overloading multiplying with single number
template<uint8_t size, typename type = float>
OurVector<size, type> operator*(OurVector<size, type>& vector, float number)
{
    for(uint8_t i = 0; i < size; ++i)
    {
        vector[i] *= number;
    }

    return vector;
}

//! Another overloading multiplying with single number
template<uint8_t size, typename type = float>
OurVector<size, type> operator*(float number, OurVector<size, type>& vector)
{
    return vector * number;
}

template<uint8_t size, typename type>
OurVector<size, type> &OurVector<size, type>::operator=(const OurVector &other)
{
    if (this != &other)
    {
        std::swap(OurVector(other), *this);
    }

    return *this;
}

template<uint8_t size, typename type>
OurVector<size, type>::OurVector(const OurVector &other) : vector(new type[size])
{
    checkSize(other);

    for(uint8_t i = 0; i < size; ++i)
    {
        this->vector[i] = other.vector[i];
    }

}

template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator+(const OurVector &other)
{
    checkSize(other);

    for(uint8_t i = 0; i < size; ++i)
    {
        this->vector[i] += other.vector[i];
    }

    return *this;
}

template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator-(const OurVector &other)
{
    checkSize(other);

    for(uint8_t i = 0; i < size; ++i)
    {
        this->vector[i] -= other.vector[i];
    }

    return *this;
}


template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator*(const OurVector& other) {
    checkSize(other);

    for(uint8_t i = 0; i < size; ++i)
    {
        this->vector[i] *= other.vector[i];
    }

    return *this;
}

#endif //MLAT_VECTOR_H
