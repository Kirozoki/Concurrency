#include <iostream>
#include <future>
#include <exception>
#include <cmath>

void print_int(std::future<int>& fut) {
    
    try
    {
        int x = fut.get();

        std::cout << x << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "[exception caught: " << e.what() << "]\n";
    }
    
}

void sqrt_calculation(std::promise<int>& prom) {
    int x = 0;

    std::cout << "Please, enter an integer value: ";
    std::cin >> x;
    try
    {
        if (x < 0) {
            throw std::invalid_argument("input cannot be negative");
        }
        prom.set_value(std::sqrt(x));
    }
    catch(std::exception& e)
    {
        prom.set_exception(std::current_exception());
    }
}

int main() {

    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread print_thread(print_int, std::ref(fut));
    std::thread calc_thread(sqrt_calculation, std::ref(prom));

    print_thread.join();
    calc_thread.join();
    
}