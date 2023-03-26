#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

typedef std::vector<std::vector<double>> Matrix;

Matrix transpose(const Matrix &matrix) {
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();
    Matrix result(cols, std::vector<double>(rows));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

Matrix matrixMultiply(const Matrix &A, const Matrix &B) {
    size_t rowsA = A.size();
    size_t colsA = A[0].size();
    size_t rowsB = B.size();
    size_t colsB = B[0].size();
    Matrix result(rowsA, std::vector<double>(colsB, 0.0));

    for (size_t i = 0; i < rowsA; ++i) {
        for (size_t j = 0; j < colsB; ++j) {
            for (size_t k = 0; k < colsA; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

void qrDecomposition(const Matrix &A, Matrix &Q, Matrix &R) {
    size_t rows = A.size();
    size_t cols = A[0].size();

    Q = Matrix(rows, std::vector<double>(rows, 0.0));
    R = Matrix(rows, std::vector<double>(cols, 0.0));

    for (size_t k = 0; k < cols; ++k) {
        double norm = 0;
        for (size_t i = 0; i < rows; ++i) {
            norm += A[i][k] * A[i][k];
        }
        norm = sqrt(norm);
        R[k][k] = norm;

        for (size_t i = 0; i < rows; ++i) {
            Q[i][k] = A[i][k] / norm;
        }

        for (size_t j = k + 1; j < cols; ++j) {
            double dot = 0;
            for (size_t i = 0; i < rows; ++i) {
                dot += A[i][j] * Q[i][k];
            }
            R[k][j] = dot;

            for (size_t i = 0; i < rows; ++i) {
                A[i][j] = A[i][j] - R[k][j] * Q[i][k];
            }
        }
    }
}

Matrix pseudoInverse(const Matrix &A) {
    Matrix Q, R, Qt, Rt;
    qrDecomposition(A, Q, R);
    Qt = transpose(Q);
    Rt = transpose(R);

    size_t rows = R.size();
    size_t cols = R[0].size();
    Matrix R_pseudo(cols, std::vector<double>(rows, 0.0));

    for (size_t i = 0; i < cols; ++i) {
        if (std::abs(R[i][i]) > std::numeric_limits<double>::epsilon()) {
            R_pseudo[i][i] = 1.0 / R[i][i];
        }
    }
	return matrixMultiply(matrixMultiply(Rt, R_pseudo), Qt);
}

void printMatrix(const Matrix &matrix) {
    for (const auto &row : matrix) {
        for (const auto &elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    Matrix A = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    std::cout << "Original matrix:" << std::endl;
    printMatrix(A);

    Matrix pseudo_inverse = pseudoInverse(A);

    std::cout << "Pseudo-inverse matrix:" << std::endl;
    printMatrix(pseudo_inverse);

    return 0;
}
