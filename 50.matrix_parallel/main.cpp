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
    const int matrix_size = 1000; 
    Matrix A(matrix_size, matrix_size);
    Matrix B(matrix_size, matrix_size);
    Matrix C(matrix_size, matrix_size);
    Matrix D(matrix_size, matrix_size);

    A.set_all(1);
    B.set_all(1);

    //seuential implementation
    auto startTime = high_resolution_clock::now();
    Matrix::multiply(&A, &B, &C);
    auto endTime = high_resolution_clock::now();
    print_results("sequential matrix multiplication", startTime, endTime);

     // custom parallel implementation
    startTime = high_resolution_clock::now();
    Matrix::parallel_multiply(&A, &B, &D);
    endTime = high_resolution_clock::now();    
    print_results("parallel matrix multiplication", startTime, endTime);

    
}