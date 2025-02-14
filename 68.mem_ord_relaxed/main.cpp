#include <atomic>
#include <assert.h>
#include <iostream>
#include <memory>
#include <thread>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y() {
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_relaxed);
}

void read_y_then_x() {
    //the first y as y was written the last above
    while (!y.load(std::memory_order_relaxed));

    // check whether x is true
    if (x.load(std::memory_order_relaxed)) { 
        // as relaxed option then there's no sequence nor guaranteed true
        //optimizaion but there's no guarantee at the end
        z++;
    }
}

void run_code() {
    x = false;
    y = false;
    z = 0;

    std::thread writer_thread(write_x_then_y);
    std::thread reader_thread(read_y_then_x);

    writer_thread.join();
    reader_thread.join();

    assert(z != 0);
}

int main() {
    run_code();
    
}