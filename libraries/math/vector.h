#ifndef MLAT_VECTOR_H
#define MLAT_VECTOR_H

#include <iostream>
#include <vector>
#include <cassert>
#include <functional>


template<uint8_t size, typename type = float>
class OurVector : public std::vector<type>
{
public:
    //!Default constructor
    OurVector() { this->resize(size); }
    //! Constructor with std::vector
    explicit OurVector(const std::vector<type>& values)
    {
        this->resize(size);
        for(uint8_t i = 0; i < size; ++i)
        {
            this->at(i) = values.at(i);
        }
    }

    //! Overloading sum with another vector
    OurVector<size, type> operator+(const OurVector& other)
    {
        checkSize(other);

        for(uint8_t i = 0; i < size; ++i)
        {
            this->at(i) += other.at(i);
        }

        return *this;
    }
    //! Overloading subtraction with another vector
    OurVector<size, type> operator-(const OurVector& other)
    {
        checkSize(other);

        for(uint8_t i = 0; i < size; ++i)
        {
            this->at(i) -= other.at(i);
        }

        return *this;
    }
    //! Overloading multiplying with single number
    OurVector<size, type> operator*(float number) {
        for(uint8_t i = 0; i < size; ++i)
        {
            this->at(i) *= number;
        }

        return *this;
    }
    //! Overloading multiplying with another vector
    OurVector<size, type> operator*(const OurVector& other)
    {
        checkSize(other);

        for(uint8_t i = 0; i < size; ++i)
        {
            this->at(i) *= other.at(i);
        }

        return *this;
    }

    //! Check size
    void checkSize(const OurVector& other)
    {
        assert(other.size() == size);
    }
};


#endif //MLAT_VECTOR_H
