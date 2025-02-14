#pragma once

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>

#include "utils.h"
#include "common_objs.h"

//could be created as a template class i.e. T
class Matrix {

private:
    int* data;
    int rows;
    int columns;

public:
    Matrix(int _n, int _m) : rows(_n), columns(_m) {
        data = new int[ rows * columns]; // array in dynamic memory
        //initialize the data array elements to 0
        std::fill(data, data + rows * columns, 0); // could be T{}    
    }

    ~Matrix() { // for ~T{} should be considered
    }

    void set_value(int row, int col, int value) { 
        // T value, row from 0 and column from 0
        data[row * columns + col] = value; 
    }

    void set_all(int value) { 
        // T value
        std::fill(data, data + rows * columns, value); 
    }

    static void multiply(Matrix* x, Matrix* y, Matrix* results) {
        
        if ((!x) || (!y) || (!results)) {
            std::cout << "Error: one of matrices is not created" 
            << std::endl;
        }
        
        if ((x->columns != y->rows) || (results->columns != y->columns)
            || (results->rows != x->rows)) {
            std::cout << "Error : Ivalid dimensions of matrices for multiplication" 
            << std::endl;
        }

        int result_array_size = results->rows * results->columns;

        for (size_t i = 0; i < result_array_size; ++i) {
            
            for(size_t j = 0; j < x->columns; ++j) {
                results->data[i] += x->data[ (i / results->columns) * x->columns + j]
                * y->data[i % results->rows + j * y->columns];
            } 
                
        }
    }

    int get_rows() const {
        return rows;
    }

    int get_columns() const {
        return columns;
    }

    int get_value(int row_i, int col_i) const { // return T
        return data[row_i * rows + col_i];
    }
};