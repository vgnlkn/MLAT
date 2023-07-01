#ifndef MLAT_MATRIX_H
#define MLAT_MATRIX_H

#include <vector>
#include <cassert>
#include <utility>
#include <cmath>
#include <vector.h>

/*! \class OurMatrix
*   \brief Class describing the matrix
*   This class based on OurVector class and extend it for using as matrixed.
*   Allowed all base operations with matrixes, except determinant calculation
*   and getting inversed matrix.
*/
template<uint8_t row, uint8_t col, typename type=double>
class OurMatrix
{
public:
    //! Default constructor
    inline OurMatrix() : _matrix(new OurVector<col, type>[row]) { setZero(); };
    //! Destructor
    inline ~OurMatrix() { delete[] _matrix; }
    //! Copy constructor
    OurMatrix(const OurMatrix& other);
    //! Get row
    OurVector<col, type> getRow(uint8_t row_index) const;
    //! Get column
    OurVector<row, type> getCol(uint8_t col_index) const;
    //! Replace all fields
    inline void swap(OurMatrix& other) { std::swap(this->_matrix, other._matrix); };
    //! Set value val to each matrix element
    void setValue(type value);
    //! Set each matrix element to 0
    inline void setZero() { this->setValue(0.f); }
    //! Gets transposed copy of matrix
    OurMatrix<col, row, type> getTransposed() const;

    //! Computing the pseudo inversed matrix
    OurMatrix<col, row, type> pseudoInverse() const;
    //! Computing the QR-decomposition of current matrix
    std::pair<OurMatrix<row, col, type>, OurMatrix<col, col, type>> QRDecomposition() const;
    //! LU decompose
    OurMatrix<row, col> matrixDecompose(OurMatrix<row, col> matrix, OurVector<row>& perm, int& toggle);
    //! Solve system
    OurVector<row> helperSolve(OurMatrix<row, col>& luMatrix, OurVector<row>& b);
    //! Inverse matrix
    OurMatrix<row, col> getLUPInverse();

    //! Set value on column
    void setColumn(uint8_t col_index, type value);
    //! Set value on row
    void setRow(uint8_t row_index, type value);

    //! ONLY FOR SQUARE MATRIXES:
    //! Set value on main diagonal of the matrix(sets 0 on other cells)
    void setDiagonalValue(type value);
    //! Set 1 on main diagonal of the matrix
    inline void setIdentity() { this->setDiagonalValue(1.f); };
    //! Transposes the matrix
    void transpose();

    //! Get row size
    [[nodiscard]] inline uint8_t getRowNumber() const { return row; }
    //! Get column size
    [[nodiscard]] inline uint8_t getColNumber() const { return col; }
    //! Find inverse matrix
    OurMatrix<col, row, type> getInverse() const;

    //! OPERATOR OVERLOADINGS
    //! Overloading operator==
    bool operator==(const OurMatrix& other) const;
    //! Overloading operator!=
    inline bool operator!=(const OurMatrix& other) const { return !(*this == other); }
    //! Overloading operator =
    OurMatrix<row, col, type>& operator=(const OurMatrix& other);
    //! Overloading operator[] with link
    inline OurVector<col, type>& operator[] (uint8_t i) const { return _matrix[i]; }
    //! Overloading sum with another matrix
    OurMatrix<row, col, type> operator+(const OurMatrix& other) const;
    //! Overloading operator- that inverts number
    OurMatrix<row, col, type> operator-() const;
    //! Overloading substraction with another matrix
    inline OurMatrix<row, col, type> operator-(const OurMatrix& other) const { return *this + (-other); };
    //! Overloading multiplying with single number (matrix first)
    OurMatrix<row, col, type> operator*(type number) const;
    //! Overloading multiplying with vector (matrix first)
    OurVector<row, type> operator*(const OurVector<col, type>& other) const;
    //! Overloading multiplying with single number(number is first)
    template<uint8_t row_, uint8_t col_, typename T>
    friend OurMatrix<row_, col_, T> operator*(double number, OurMatrix<row_, col_, T>& vector);
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
    void checkParams(const OurMatrix& other) const;

private:
    OurVector<col, type>* _matrix;

};

template<uint8_t row, uint8_t col, typename type>
OurMatrix<row, col> OurMatrix<row, col, type>::getLUPInverse()
{
    const int n = row;
    OurMatrix<row, col> result = *this;
    OurVector<n> perm;
    int toggle;
    auto copy = *this;
    auto lum = matrixDecompose(copy, perm, toggle);

    OurVector<n> b;
    for (uint8_t i = 0; i < n; ++i)
    {
        for (uint8_t j = 0; j < n; ++j)
        {
            if (i == perm[j])
            {
                b[j] = 1.0;
            }
            else
            {
                b[j] = 0.0;
            }
        }

        OurVector<n> x = helperSolve(lum, b);
        for (uint8_t j = 0; j < n; ++j)
        {
            result[j][i] = x[j];
        }
    }

    return result;
}

template<uint8_t row, uint8_t col, typename type>
OurVector<row> OurMatrix<row, col, type>::helperSolve(OurMatrix<row, col> &luMatrix, OurVector<row> &b)
{
    const uint8_t n = row;
    OurVector<n> x = b;

    for (uint8_t i = 1; i < n; ++i)
    {
        double sum = x[i];
        for (uint8_t j = 0; j < i; ++j)
        {
            sum -= luMatrix[i][j] * x[j];
        }
        x[i] = sum;
    }

    x[n - 1] /= luMatrix[n - 1][n - 1];

    for (int i = n - 2; i >= 0; --i)
    {
        auto idx_i = static_cast<uint8_t>(i);
        double sum = x[idx_i];
        for (uint8_t j = idx_i + 1; j < n; ++j)
        {
            sum -= luMatrix[idx_i][j] * x[j];
        }
        x[idx_i] = sum / luMatrix[idx_i][idx_i];
    }

    return x;
}

template<uint8_t row, uint8_t col, typename type>
OurMatrix<row, col> OurMatrix<row, col, type>::matrixDecompose(OurMatrix<row, col> matrix, OurVector<row> &perm, int &toggle)
{
    const int n = row;
    OurMatrix<row, col> result = matrix;
    for (uint8_t i = 0; i < n; ++i)
    {
        perm[i] = i;
    }
    toggle = 1;
    for (uint8_t j = 0; j < n - 1; ++j)
    {
        double colMax = std::abs(result[j][j]);
        uint8_t pRow = j;
        for (uint8_t i = j + 1; i < n; ++i)
        {
            if (result[i][j] > colMax)
            {
                colMax = result[i][j];
                pRow = i;
            }
        }
        if (pRow != j)
        {
            OurVector<row> rowPtr = result[pRow];
            result[pRow] = result[j];
            result[j] = rowPtr;
            double tmp = perm[pRow];
            perm[pRow] = perm[j];
            perm[j] = tmp;
            toggle = -toggle;
        }
        if (std::abs(result[j][j]) < 1.0E-20)
        {
            return OurMatrix<row, col>();
        }
        for (uint8_t i = j + 1; i < n; ++i)
        {
            result[i][j] /= result[j][j];
            for (uint8_t k = j + 1; k < n; ++k)
            {
                result[i][k] -= result[i][j] * result[j][k];
            }
        }
    }

    return result;
}

template<uint8_t row, uint8_t col, typename type>
OurMatrix<col, row, type> OurMatrix<row, col, type>::getInverse() const
{
    checkParams(*this);
    assert(row == col);

    OurMatrix<row, 2 * col, type> augmented_matrix;
    augmented_matrix.setZero();

    for (uint8_t i = 0; i < row; ++i)
    {
        for (uint8_t j = 0; j < col; ++j)
        {
            augmented_matrix[i][j] = (*this)[i][j];
        }
        augmented_matrix[i][i + col] = 1;
    }

    for (uint8_t i = 0; i < col; ++i)
    {
        if ((*this)[i][i] == 0)
        {
            for (uint8_t j = i + 1; j < row; ++j)
            {
                if (augmented_matrix[j][i] != 0)
                {
                    std::swap(augmented_matrix[i], augmented_matrix[j]);
                    break;
                }
            }
        }

        type pivot = augmented_matrix[i][i];
        for (uint8_t j = i; j < 2 * col; ++j)
        {
            augmented_matrix[i][j] /= pivot;
        }

        for (uint8_t j = 0; j < row; ++j)
        {
            if (j != i)
            {
                type factor = augmented_matrix[j][i];
                for (uint8_t k = i; k < 2 * col; ++k)
                {
                    augmented_matrix[j][k] -= factor * augmented_matrix[i][k];
                }
            }
        }
    }

    OurMatrix<col, row, type> inverse_matrix;
    for (uint8_t i = 0; i < col; ++i)
    {
        for (uint8_t j = row; j < 2 * col; ++j)
        {
            inverse_matrix[i][j - row] = augmented_matrix[i][j];
        }
    }

    return inverse_matrix;
}


template<uint8_t row1, uint8_t col1, uint8_t row2, uint8_t col2, typename T>
OurMatrix<row1, col2, T> operator*(const OurMatrix<row1, col1, T>& first, const OurMatrix<row2, col2, T>& second)
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
inline OurMatrix<row_, col_, T> operator*(double number, OurMatrix<row_, col_, T>& vector)
{
    return vector * number;
}

template<uint8_t row_, uint8_t col_, typename T>
inline OurVector<col_, T> operator*(const OurVector<row_, T>& vector, const OurMatrix<row_, col_, T>& matrix)
{
    OurVector<col_, T> product;
    double sum = 0;
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
    for (uint8_t i = 0; i < row; ++i)
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
    for (uint8_t i = 0; i < row; ++i)
    {
        for (uint8_t j = 0; j < col; ++j)
        {
            transposed[j][i] = _matrix[i][j];
        }
    }
    return transposed;
}

template<uint8_t row, uint8_t col, typename type>
inline OurMatrix<col, row, type> OurMatrix<row, col, type>::pseudoInverse() const
{
    std::pair<OurMatrix<row, col, type>, OurMatrix<col, col, type>> QR = this->QRDecomposition();

    OurMatrix<col, row, type> Q_transposed = QR.first.getTransposed();
    OurMatrix<col, col, type> R_inversed = QR.second;

    double sum;
    const int column = col;
    for (int i = column - 1; i >= 0; i--)
    {
        auto idx_i = static_cast<uint8_t>(i);
        R_inversed[idx_i][idx_i] = 1 / QR.second[idx_i][idx_i];
        for (int j = i - 1; j >= 0; j--)
        {
            auto idx_j = static_cast<uint8_t>(j);
            sum = 0;
            for (int k = j + 1; k <= i; k++)
            {
                auto idx_k = static_cast<uint8_t>(k);
                sum += QR.second[idx_j][idx_k] * R_inversed[idx_k][idx_i];
            }
            R_inversed[idx_j][idx_i] = -sum / QR.second[idx_j][idx_j];
        }
    }


    return R_inversed * Q_transposed;
}

template<uint8_t row, uint8_t col, typename type>
inline std::pair<OurMatrix<row, col, type>, OurMatrix<col, col, type>> OurMatrix<row, col, type>::QRDecomposition() const
{
    OurMatrix<row, col, type> Q;
    OurMatrix<col, col, type> R;
    for (uint8_t j = 0; j < col; j++)
    {
        for (uint8_t i = 0; i < row; i++)
        {
            Q[i][j] = _matrix[i][j];
        }
        for (uint8_t k = 0; k < j; k++)
        {
            double dot = 0;
            for (uint8_t i = 0; i < row; i++)
            {
                dot += Q[i][k] * _matrix[i][j];
            }
            R[k][j] = dot;
            for (uint8_t i = 0; i < row; i++)
            {
                Q[i][j] -= R[k][j] * Q[i][k];
            }
        }
        double norm = 0;
        for (uint8_t i = 0; i < row; i++)
        {
            norm += Q[i][j] * Q[i][j];
        }
        norm = std::sqrt(norm);
        for (uint8_t i = 0; i < row; i++)
        {
            Q[i][j] /= norm;
        }
        R[j][j] = norm;
    }

    return std::make_pair(Q, R);
}

template<uint8_t row, uint8_t col, typename type>
inline void OurMatrix<row, col, type>::setDiagonalValue(type value)
{
    assert(row == col);
    this->setZero();
    for (uint8_t y = 0; y < row; ++y)
    {
        _matrix[y][y] = value;
    }
}

template<uint8_t row, uint8_t col, typename type>
inline void OurMatrix<row, col, type>::transpose()
{
    assert(row == col);
    for (uint8_t i = 1; i < row; ++i)
    {
        for (uint8_t j = 0; j < i; ++j)
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
    for (uint8_t y = 0; y < row; ++y)
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
    for (uint8_t y = 0; y < row; ++y)
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
inline void OurMatrix<row, col, type>::checkParams(const OurMatrix& other) const {
    assert(std::is_arithmetic<type>() &&
           typeid(_matrix).name() == typeid(other._matrix).name());
}

#endif