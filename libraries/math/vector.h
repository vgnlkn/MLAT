#ifndef MLAT_VECTOR_H
#define MLAT_VECTOR_H

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <typeinfo>

/*! \class OurVector
*   \brief Class describing the custom vector
*/
template<uint8_t size, typename type=long double>
class OurVector final
{
public:
    //! Default constructor
    OurVector() : _vector(new type[size]) { setValue(0.f); }
    //! Copy constructor
    OurVector(const OurVector& other);
    //! Overloading operator=
    OurVector<size, type>& operator=(const OurVector& other);
    //! Destructor
    ~OurVector() { if (_vector) { delete[] _vector; } };

    //! Set vector values to val
    void setValue(type value);
    //! Replace all fields
    void swap(OurVector& other) { std::swap(this->_vector, other._vector); };
    //! Get size
    [[nodiscard]] uint8_t inline getSize() const { return size; }

    //! OPERATOR OVERLOADINGS
    //! Overloading operator==
    bool operator==(const OurVector& other) const;
    //! Overloading operator!=
    bool operator!=(const OurVector& other) const;
    //! Overloading operator[] with link
    type& operator[](uint8_t i) const { return _vector[i]; }
    //! Overloading sum with another _vector
    OurVector<size, type> operator+(const OurVector& other) const;
    //! Overloading multiplying with another _vector
    OurVector<size, type> operator*(const OurVector& other) const;
    //! Overloading subtraction with another _vector
    OurVector<size, type> operator-(OurVector other) const;
    //! Overloading operator- that inverts number
    OurVector<size, type> operator-() const;
    //! Overloading multiplying with single number
    OurVector<size, type> operator*(long double number) const;
    //! Overloading multiplying with single number(number is first)
    template<uint8_t size_, typename T>
    friend OurVector<size_, T> operator*(long double number, OurVector<size_, T>& vector);
    //! Overloading output to stream operator
    template<uint8_t size_, typename T>
    friend std::ostream& operator<<(std::ostream& os, const OurVector<size_, T>& vector);

private:
    //! Check all parameters
    void checkParams(const OurVector& other) const;

private:
    type* _vector;
};


template<uint8_t size, typename type>
inline void OurVector<size, type>::setValue(type value)
{
    for (int i = 0; i < size; ++i)
    {
        _vector[i] = value;
    }
}

template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator*(long double number) const
{
    OurVector<size, type> result;

    for(uint8_t i = 0; i < size; ++i)
    {
        result._vector[i] = this->_vector[i] * number;
    }

    return result;
}

template<uint8_t size_, typename T>
OurVector<size_, T> operator*(long double number, OurVector<size_, T>& vector)
{
    return vector * number;
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
    checkParams(other);

    for(uint8_t i = 0; i < size; ++i)
    {
        this->_vector[i] = other._vector[i];
    }

}

template<uint8_t size, typename type>
bool OurVector<size, type>::operator==(const OurVector &other) const
{
    checkParams(other);

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
bool OurVector<size, type>::operator!=(const OurVector &other) const
{
    return !(*this == other);
}

template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator+(const OurVector &other) const
{
    checkParams(other);
    OurVector<size, type> result;

    for(uint8_t i = 0; i < size; ++i)
    {
        result[i] = this->_vector[i] + other._vector[i];
    }

    return result;
}

template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator-() const
{
    return *this * (-1);
}

template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator-(OurVector other) const
{
    return *this + (-other);
}


template<uint8_t size, typename type>
OurVector<size, type> OurVector<size, type>::operator*(const OurVector& other) const
{
    checkParams(other);
    OurVector<size, type> result;

    for(uint8_t i = 0; i < size; ++i)
    {
        result[i] = this->_vector[i] * other._vector[i];
    }

    return result;
}

template<uint8_t size_, typename T>
std::ostream &operator<<(std::ostream &os, const OurVector<size_, T> &vector)
{
    for(uint8_t i = 0; i < size_; ++i)
    {
        os << vector._vector[i] << ' ';
    }

    return os;
}

template<uint8_t size, typename type>
void OurVector<size, type>::checkParams(const OurVector &other) const
{
    assert(std::is_arithmetic<type>() &&
           typeid(_vector).name() == typeid(other._vector).name());
    assert(other.getSize() == size);
}

#endif //MLAT_VECTOR_H
