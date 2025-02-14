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

    static void parallel_multiply(Matrix* x, Matrix* y, Matrix* results) {

        struct process_data_chunk {
            void operator()(Matrix* x, Matrix* y, Matrix* results,
                    int start_index, int end_index){
                for (size_t i = start_index; i < end_index; ++i) {
            
                    for(size_t j = 0; j < x->columns; ++j) {
                        results->data[i] += x->data[ (i / results->columns) * x->columns + j]
                            * y->data[i % results->rows + j * y->columns];
                    } 
                }
            }
        };
        
        int length = results->rows * results->columns;

        if (!length) {
            return;
        }

        int min_per_thread = 10000;
        int max_threads = (length + min_per_thread - 1) / min_per_thread;
        int hardware_threads = std::thread::hardware_concurrency();
        int num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
        int block_size = length / num_threads;

        std::vector<std::thread> threads(num_threads - 1);
        int block_start = 0;
        int block_end = 0;
        {
            join_threads join(threads);
            for (unsigned long i = 0; i < (num_threads - 1); ++i) {
                block_start = i * block_size;
                block_end = block_start + block_size;
                threads[i] = std::thread(process_data_chunk(), x
                        , y, results, block_start, block_end);
            }
            // perform for final block
            process_data_chunk()(x, y, results, block_end, length);
        }

    }

    static void transpose(Matrix* x, Matrix* result) {
        if ((x->columns != result->rows) || (x->rows != result->columns)) {
            std::cout << " ERROR : Invalid matrix sizes for transpose \n";
            return;
        }
        int result_size = result->rows * result->columns;

        int result_column = 0;
        int result_row = 0;

        int input_column = 0;
        int input_row = 0;

        for (size_t i = 0; i < result_size; ++i) {
            //get the current row and column indeces
            result_row = i / result->columns;
            result_column = i % result->columns;

            //flipped the columns and row for input
            input_row = result_column;
            input_column = result_row;

            //store the corresponding element from input to the results
            result->data[i] = x->data[input_row * x->columns + input_column];
        }
    }

    static void parallel_transpose(Matrix* x, Matrix* result) {
        
        struct process_data_chunk{
            void operator()(Matrix* x, Matrix* result, int start_index, 
                    int end_index){
                
                int result_column = 0;
                int result_row = 0;

                int input_column = 0;
                int input_row = 0;

                for (size_t i = start_index; i < end_index; ++i) {
                //get the current row and column indeces
                    result_row = i / result->columns;
                    result_column = i % result->columns;

                //flipped the columns and row for input
                    input_row = result_column;
                    input_column = result_row;

                //store the corresponding element from input to the results
                    result->data[i] = x->data[input_row * x->columns + input_column];
                }
            }
        };

        int length = result->rows * result->columns;

        if (!length) {
            return;
        }

        int min_per_thread = 10000;
        int max_threads = (length + min_per_thread - 1) / min_per_thread;
        int hardware_threads = std::thread::hardware_concurrency();
        int num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
        int block_size = length / num_threads;
    
        std::vector<std::thread> threads(num_threads - 1);
        int block_start = 0;
        int block_end = 0;
        {
            join_threads join(threads);
            for (unsigned long i = 0; i < (num_threads - 1); ++i) {
                block_start = i * block_size;
                block_end = block_start + block_size;
                threads[i] = std::thread(process_data_chunk(), x
                        , result, block_start, block_end);
            }
            // perform for final block
            process_data_chunk()(x, result, block_end, length);
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