#include <iostream>

#include "matrix.h"

Matrix GetMatrix(int rows, int cols, int first_value) {
    Matrix matrix(rows, cols);
    for (size_t i = 0; i < rows * cols; ++i) {
        matrix.set_value(i / cols, i % cols, first_value++);
    }
    return matrix;
}

void operator<<(std::ostream& out, Matrix mx) {
    for (size_t i = 0; i < mx.get_rows() * mx.get_columns(); ++i) {
        if (i % mx.get_columns() == 0 ) out << std::endl;
        std::cout << mx.get_value(i / mx.get_columns(), i % mx.get_columns()) << " ";
    }
    out << std::endl;
}

int main() {
    Matrix mx1 = GetMatrix(3,3,1);
    Matrix mx2 = GetMatrix(3,3,0);
    Matrix mx3(3,3);
    std::cout << mx1;
    std::cout << mx2;
    Matrix::multiply(&mx1, &mx2, &mx3);

    std::cout << mx3;

    Matrix A(10,10);
    Matrix B(10,10);

    Matrix result(10,10);

    A.set_all(1);
    B.set_all(1);

    Matrix::multiply(&A, &B, &result);

    std::cout << result;

    
}