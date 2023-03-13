#ifndef MLAT_VECTOR_H
#define MLAT_VECTOR_H

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>


/*! \class OurVector
*   \brief Class describing the custom vector
*/
template<uint8_t size, typename type = float>
class OurVector
{
public:
    //!Default constructor
    OurVector() : _vector(new type[size]) {}
    //! Copy constructor
    OurVector(const OurVector& other);
    //! Overloading operator -
    OurVector<size, type>& operator=(const OurVector& other);
    //! Destructor
    ~OurVector();

    //! Replace all fields
    void swap(OurVector& other);

    //! Overloading operator==
    bool operator==(const OurVector& other);
    //! Overloading operator!=
    bool operator!=(const OurVector& other);

    //! Overloading operator[] with link
    type& operator[](uint8_t i) { return _vector[i]; }

    //! Overloading sum with another _vector
    OurVector<size, type> operator+(const OurVector& other);
    //! Overloading multiplying with another _vector
    OurVector<size, type> operator*(const OurVector& other);
    //! Overloading subtraction with another _vector
    OurVector<size, type> operator-(OurVector other);
    //! Overloading operator- that inverts number
    OurVector<size, type> operator-();

    //! Overloading multiplying with single number
    OurVector<size, type> operator*(float number);
    //! Overloading multiplying with single number(number is first)
    template<uint8_t size_, typename T>
    friend OurVector<size_, T> operator*(float number, OurVector<size_, T>& vector);

    //! Get size
    [[nodiscard]] uint8_t inline getSize() const { return size; }
    //! Check size
    void checkSize(const OurVector& other) { assert(other.getSize() == size); }
private:
    type* _vector;
};


template<uint8_t size, typename type>
OurVector<size, type>::~OurVector()
{
    if (_vector)
    {
        delete[] _vector;
    }
}

template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator*(float number)
{
    OurVector<size, type> result;

    for(uint8_t i = 0; i < size; ++i)
    {
        result._vector[i] = this->_vector[i] * number;
    }

    return result;
}

template<uint8_t size_, typename T>
OurVector<size_, T> operator*(float number, OurVector<size_, T>& vector)
{
    return vector * number;
}

template<uint8_t size, typename type>
void OurVector<size, type>::swap(OurVector &other)
{
    std::swap(this->_vector, other._vector);
}

template<uint8_t size, typename type>
OurVector<size, type> &OurVector<size, type>::operator=(const OurVector &other)
{
    if (this != &other)
    {
        OurVector(other).swap(*this);
    }

    return *this;
}

template<uint8_t size, typename type>
OurVector<size, type>::OurVector(const OurVector &other) : _vector(new type[size])
{
    checkSize(other);

    for(uint8_t i = 0; i < size; ++i)
    {
        this->_vector[i] = other._vector[i];
    }

}

template<uint8_t size, typename type>
bool OurVector<size, type>::operator==(const OurVector &other)
{
    checkSize(other);

    for(uint8_t i = 0; i < size; ++i)
    {
        if (this->_vector[i] != other._vector[i])
        {
            return false;
        }
    }

    return true;
}

template<uint8_t size, typename type>
bool OurVector<size, type>::operator!=(const OurVector &other)
{
    return *this == other;
}

template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator+(const OurVector &other)
{
    checkSize(other);
    OurVector<size, type> result;

    for(uint8_t i = 0; i < size; ++i)
    {
        result[i] = this->_vector[i] + other._vector[i];
    }

    return result;
}

template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator-()
{
    return *this * (-1);
}

template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator-(OurVector other)
{
    return *this + (-other);
}


template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator*(const OurVector& other)
{
    checkSize(other);
    OurVector<size, type> result;

    for(uint8_t i = 0; i < size; ++i)
    {
        result[i] = this->_vector[i] * other._vector[i];
    }

    return result;
}

#endif //MLAT_VECTOR_H
