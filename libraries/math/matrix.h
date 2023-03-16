#ifndef MLAT_MATRIX_H
#define MLAT_MATRIX_H

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <typeinfo>
#include <vector.h>

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
	OurVector<col, type> getRow(const uint8_t row_number) const;
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

	//! ONLY FOR SQUARE MATRIXES:
	//! Set value on main diagonal of the matrix
	void setDiagonalValue(type value);
	//! Set 1 on main diagonal of the matrix
	void setIdentity() { this->setDiagonalValue(1.f); };
	//! Transposes the matrix
	void transpose();

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
	//! Overloading operator <<
	template<uint8_t row_, uint8_t col_, typename T>
	friend std::ostream& operator<<(std::ostream& os, const OurMatrix<row_, col_, T>& matrix);

private:
	void checkParams(const OurMatrix& other);

private:
	OurVector<col, type>* _matrix;

};

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