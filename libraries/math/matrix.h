#ifndef MLAT_MATRIX_H
#define MLAT_MATRIX_H

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <typeinfo>
#include "vector.h"

/*! \class OurMatrix
*   \brief Class describing the matrix
* This class based on OurVector class and extend it for using as matrixed.
* Allowed all base operations with matrixes, except determinant calculation
* and getting inversed matrix.
*/
template<uint8_t row, uint8_t col, typename type=float>
class OurMatrix
{
public:
    //! Default constructor
    OurMatrix() : _matrix(new OurVector<col, type>[row]) { setZero(); };
    //! Destructor
    ~OurMatrix() { if (_matrix) { delete[] _matrix; } }
    //! Copy constructor
    OurMatrix(const OurMatrix& other);
    //! Get row
    OurVector<col, type> getRow(const uint8_t row_index) const;
    //! Get column
    OurVector<row, type> getCol(const uint8_t col_index) const;
    //! Replace all fields
    void swap(OurMatrix& other) { std::swap(this->_matrix, other._matrix); };
    //! Set value val to each matrix element
    void setValue(type value);
    //! Set each matrix element to 0
    void setZero() { this->setValue(0.f); }
    //! Gets transposed copy of matrix
    OurMatrix<col, row, type> getTransposed() const;
    //! Classical matrix multiplication algorithm
    template<uint8_t row1, uint8_t col1, uint8_t row2, uint8_t col2, typename T>
    friend OurMatrix<row1, col2, T> classicAlgMultiplication(const OurMatrix<row1, col1, T>& first,
                                                             const OurMatrix<row2, col2, T>& second);

    //! Set value on column
    void setColumn(uint8_t col_index, type value);
    //! Set value on row
    void setRow(uint8_t row_index, type value);

    //! ONLY FOR SQUARE MATRIXES:
    //! Set value on main diagonal of the matrix(sets 0 on other cells)
    void setDiagonalValue(type value);
    //! Set 1 on main diagonal of the matrix
    void setIdentity() { this->setDiagonalValue(1.f); };
    //! Transposes the matrix
    void transpose();
    /**Strassen's algorithm
     * Strassen's algorithm for matrix multiplication works for square matrices
     * with dimensions that are powers of two. If the dimensions of the matrices
     * are not a power of two, then they can be padded with zeros to the next
     * power of two.*/
    template<uint8_t row1, uint8_t col1, uint8_t row2, uint8_t col2, typename T>
    friend OurMatrix<row1, col2, T> strassenAlg(const OurMatrix<row1, col1, T>& first,
                                                const OurMatrix<row2, col2, T>& second);

    //! Get row size
    [[nodiscard]] inline uint8_t getRowNumber() const { return row; }
    //! Get column size
    [[nodiscard]] inline uint8_t getColNumber() const { return col; }

    //! OPERATOR OVERLOADINGS
    //! Overloading operator==
    bool operator==(const OurMatrix& other) const;
    //! Overloading operator!=
    bool operator!=(const OurMatrix& other) const { return !(*this == other); }
    //! Overloading operator =
    OurMatrix<row, col, type>& operator=(const OurMatrix& other);
    //! Overloading operator[] with link
    OurVector<col, type>& operator[] (uint8_t i) const { return _matrix[i]; }
    //! Overloading sum with another matrix
    OurMatrix<row, col, type> operator+(const OurMatrix& other) const;
    //! Overloading operator- that inverts number
    OurMatrix<row, col, type> operator-() const;
    //! Overloading substraction with another matrix
    OurMatrix<row, col, type> operator-(const OurMatrix& other) const { return *this + (-other); };
    //! Overloading multiplying with single number (matrix first)
    OurMatrix<row, col, type> operator*(type number) const;
    //! Overloading multiplying with vector (matrix first)
    OurVector<row, type> operator*(const OurVector<col, type>& other) const;
    //! Overloading multiplying with single number(number is first)
    template<uint8_t row_, uint8_t col_, typename T>
    friend OurMatrix<row_, col_, T> operator*(float number, OurMatrix<row_, col_, T>& vector);
    //! Overloading multiplying with vector (vector first)
    template<uint8_t row_, uint8_t col_, typename T>
    friend OurVector<col_, T> operator*(const OurVector<row_, T>& vector, const OurMatrix<row_, col_, T>& matrix);
    //! Overloading multiplying with another matrix
    template<uint8_t row1, uint8_t col1, uint8_t row2, uint8_t col2, typename T>
    friend OurMatrix<row1, col2, T> operator*(const OurMatrix<row1, col1, T>& first,
                                              const OurMatrix<row2, col2, T>& second);
    //! Overloading operator <<
    template<uint8_t row_, uint8_t col_, typename T>
    friend std::ostream& operator<<(std::ostream& os, const OurMatrix<row_, col_, T>& matrix);

private:
    void checkParams(const OurMatrix& other);

public:
    OurVector<col, type>* _matrix;

};


template<uint8_t row1, uint8_t col1, uint8_t row2, uint8_t col2, typename T>
OurMatrix<row1, col2, T> operator*(const OurMatrix<row1, col1, T>& first, const OurMatrix<row2, col2, T>& second)
{

    if (row1 == col1 == row2 == col2)
    {
        return strassenAlg(first, second);
    }
    return classicAlgMultiplication(first, second);
}


template<uint8_t row1, uint8_t col1, uint8_t row2, uint8_t col2, typename T>
OurMatrix<row1, col2, T> classicAlgMultiplication(const OurMatrix<row1, col1, T>& first,
                                                  const OurMatrix<row2, col2, T>& second)
{
    assert(col1 == row2);

    const uint8_t n = row1;
    const uint8_t m = col2;
    const uint8_t p = col1;

    OurMatrix<row1, col2, T> result;
    for (uint8_t i = 0; i < n; ++i)
    {
        for (uint8_t j = 0; j < m; ++j)
        {
            for (uint8_t k = 0; k < p; ++k)
            {
                result[i][j] += first._matrix[i][k] * second._matrix[k][j];
            }
        }
    }

    return result;
}

template<uint8_t row1, uint8_t col1, uint8_t row2, uint8_t col2, typename T>
OurMatrix<row1, col2, T> strassenAlg(const OurMatrix<row1, col1, T>& first,
                                     const OurMatrix<row2, col2, T>& second)
{
    const uint8_t n = row1;
    OurMatrix<n, n, T> result;
    if (n == 1)
    {
        result._matrix[0][0] = first._matrix[0][0] * second._matrix[0][0];
        return result;
    }

    const uint8_t m = n / 2;
    OurMatrix<m, m, T> A11, A12, A21, A22;
    OurMatrix<m, m, T> B11, B12, B21, B22;
    for (uint8_t i = 0; i < m; i++)
    {
        for (uint8_t j = 0; j < m; j++)
        {
            A11._matrix[i][j] = first._matrix[i][j];
            A12._matrix[i][j] = first._matrix[i][j + m];
            A21._matrix[i][j] = first._matrix[i + m][j];
            A22._matrix[i][j] = first._matrix[i + m][j + m];
            B11._matrix[i][j] = second._matrix[i][j];
            B12._matrix[i][j] = second._matrix[i][j + m];
            B21._matrix[i][j] = second._matrix[i + m][j];
            B22._matrix[i][j] = second._matrix[i + m][j + m];
        }
    }

    OurMatrix<m, m> S1, S2, S3, S4, S5, S6, S7, S8, S9, S10;

    for (uint8_t i = 0; i < m; i++)
    {
        for (uint8_t j = 0; j < m; j++)
        {
            S1._matrix[i][j] = B12._matrix[i][j] - B22._matrix[i][j];
            S2._matrix[i][j] = A11._matrix[i][j] + A12._matrix[i][j];
            S3._matrix[i][j] = A21._matrix[i][j] + A22._matrix[i][j];
            S4._matrix[i][j] = B21._matrix[i][j] - B11._matrix[i][j];
            S5._matrix[i][j] = A11._matrix[i][j] + A22._matrix[i][j];
            S6._matrix[i][j] = B11._matrix[i][j] + B22._matrix[i][j];
            S7._matrix[i][j] = A12._matrix[i][j] - A22._matrix[i][j];
            S8._matrix[i][j] = B21._matrix[i][j] + B22._matrix[i][j];
            S9._matrix[i][j] = A11._matrix[i][j] - A21._matrix[i][j];
            S10._matrix[i][j] = B11._matrix[i][j] + B12._matrix[i][j];
        }
    }

    auto P1 = strassenAlg(A11, S1);
    auto P2 = strassenAlg(S2, B22);
    auto P3 = strassenAlg(S3, B11);
    auto P4 = strassenAlg(A22, S4);
    auto P5 = strassenAlg(S5, S6);
    auto P6 = strassenAlg(S7, S8);
    auto P7 = strassenAlg(S9, S10);
    for (uint8_t i = 0; i < m; i++)
    {
        for (uint8_t j = 0; j < m; j++)
        {
            result._matrix[i][j] = P5._matrix[i][j] + P4._matrix[i][j]
                                   - P2._matrix[i][j] + P6._matrix[i][j];
            result._matrix[i][j + m] = P1._matrix[i][j] + P2._matrix[i][j];
            result._matrix[i + m][j] = P3._matrix[i][j] + P4._matrix[i][j];
            result._matrix[i + m][j + m] = P5._matrix[i][j] + P1._matrix[i][j]
                                           - P3._matrix[i][j] - P7._matrix[i][j];
        }
    }

    return result;
}

template<uint8_t row, uint8_t col, typename type>
void OurMatrix<row, col, type>::setColumn(uint8_t col_index, type value)
{
    assert(col_index < col);
    for (uint8_t i = 0; i < row; ++i)
    {
        _matrix[i][col_index] = value;
    }
}

template<uint8_t row, uint8_t col, typename type>
void OurMatrix<row, col, type>::setRow(uint8_t row_index, type value)
{
    assert(row_index < row);

    for (uint8_t i = 0; i < col; ++i)
    {
        _matrix[row_index][i] = value;
    }
}

template<uint8_t row_, uint8_t col_, typename T>
inline OurMatrix<row_, col_, T> operator*(float number, OurMatrix<row_, col_, T>& vector)
{
    return vector * number;
}

template<uint8_t row_, uint8_t col_, typename T>
inline OurVector<col_, T> operator*(const OurVector<row_, T>& vector, const OurMatrix<row_, col_, T>& matrix)
{
    OurVector<col_, T> product;
    float sum = 0;
    for (int8_t x = 0; x < col_; ++x)
    {
        for (int8_t y = 0; y < row_; ++y)
        {
            sum += vector[y] * matrix[y][x];
        }
        product[x] = sum;
        sum = 0;
    }
    return product;
}

template<uint8_t row_, uint8_t col_, typename T>
inline std::ostream& operator<<(std::ostream& os, const OurMatrix<row_, col_, T>& matrix)
{
    for (int y = 0; y < row_; ++y)
    {
        os << matrix._matrix[y];
        if (y + 1 != row_)
        {
            os << std::endl;
        }
    }
    return os;
}

template<uint8_t row, uint8_t col, typename type>
inline OurMatrix<row, col, type>::OurMatrix(const OurMatrix& other):
        _matrix(new OurVector<col,type>[row])
{
    checkParams(other);
    for (int8_t y = 0; y < row; ++y)
    {
        _matrix[y] = other._matrix[y];
    }

}

template<uint8_t row, uint8_t col, typename type>
inline OurVector<col, type> OurMatrix<row, col, type>::getRow(const uint8_t row_index) const
{
    assert(row_index < row);
    return _matrix[row_index];
}

template<uint8_t row, uint8_t col, typename type>
inline OurVector<row, type> OurMatrix<row, col, type>::getCol(const uint8_t col_index) const
{
    assert(col_index < col);
    OurVector<row, type> column;
    for (int i = 0; i < row; ++i)
    {
        column[i] = _matrix[i][col_index];
    }

    return column;
}

template<uint8_t row, uint8_t col, typename type>
inline void OurMatrix<row, col, type>::setValue(type value)
{
    for (int8_t y = 0; y < row; ++y)
    {
        _matrix[y].setValue(value);
    }
}

template<uint8_t row, uint8_t col, typename type>
inline OurMatrix<col, row, type> OurMatrix<row, col, type>::getTransposed() const
{
    OurMatrix<col, row, type> transposed;
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            transposed[j][i] = _matrix[i][j];
        }
    }
    return transposed;
}

template<uint8_t row, uint8_t col, typename type>
inline void OurMatrix<row, col, type>::setDiagonalValue(type value)
{
    assert(row == col);
    this->setZero();
    for (int y = 0; y < row; ++y)
    {
        _matrix[y][y] = value;
    }
}

template<uint8_t row, uint8_t col, typename type>
inline void OurMatrix<row, col, type>::transpose()
{
    assert(row == col);
    for (int i = 1; i < row; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            std::swap(_matrix[i][j], _matrix[j][i]);
        }
    }
}

template<uint8_t row, uint8_t col, typename type>
inline bool OurMatrix<row, col, type>::operator==(const OurMatrix& other) const
{
    for (int8_t y = 0; y < row; ++y)
    {
        if (_matrix[y] != other._matrix[y])
        {
            return false;
        }
    }
    return true;
}

template<uint8_t row, uint8_t col, typename type>
inline OurMatrix<row, col, type>& OurMatrix<row, col, type>::operator=(const OurMatrix& other)
{
    if (this != &other)
    {
        OurMatrix(other).swap(*this);
    }
    return *this;
}

template<uint8_t row, uint8_t col, typename type>
inline OurMatrix<row, col, type> OurMatrix<row, col, type>::operator+(const OurMatrix& other) const
{
    OurMatrix<row, col, type> prod;
    for (int8_t y = 0; y < row; ++y)
    {
        prod[y] = _matrix[y] + other[y];
    }
    return prod;
}

template<uint8_t row, uint8_t col, typename type>
inline OurMatrix<row, col, type> OurMatrix<row, col, type>::operator-() const
{
    OurMatrix<row, col, type> prod;
    for (int8_t y = 0; y < row; ++y)
    {
        prod[y] = -_matrix[y];
    }
    return prod;
}

template<uint8_t row, uint8_t col, typename type>
inline OurMatrix<row, col, type> OurMatrix<row, col, type>::operator*(type number) const
{
    OurMatrix<row, col, type> prod;
    for (int8_t y = 0; y < row; ++y)
    {
        prod[y] = _matrix[y] * number;
    }
    return prod;
}

template<uint8_t row, uint8_t col, typename type>
inline OurVector<row, type> OurMatrix<row, col, type>::operator*(const OurVector<col, type>& other) const
{
    OurVector<row, type> product;
    type item = 0;
    for (int8_t y = 0; y < row; ++y)
    {
        for (int8_t x = 0; x < col; ++x)
        {
            item += _matrix[y][x] * other[x];
        }
        product[y] = item;
        item = 0;
    }
    return product;
}

template<uint8_t row, uint8_t col, typename type>
inline void OurMatrix<row, col, type>::checkParams(const OurMatrix& other)
{
    assert(std::is_arithmetic<type>() &&
           typeid(_matrix).name() == typeid(other._matrix).name());
}

#endif