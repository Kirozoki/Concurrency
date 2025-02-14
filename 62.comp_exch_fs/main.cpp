/*
compare_exchange_weak
compare_exchange_strong

Both compare_exchange 

bool r = x.compare_exchange_weak(T& expected, T desired);

compare values of 'x' and 'expected' and 
store the supplied 'desired' value if they are equal. 
if they are NOT then 'expected' value is updated with the actual value of the atomic variable 'x'. 
this will return 'true' if store is performed and 'false' otherwise.

compare_exchange_weak is differ from compare_exchange_strong in a way that success is not guaranteed 
even if values are equal. it's due to machine operation doesn't have single compare exchange instruction
then processor will not guarantee to be atomic operation. 

compare_exchange_strong guarantees to be atomic operation if values are equal.
*/
#include <iostream>
#include <thread>
#include <atomic>

void run_code1() {
    std::atomic<int> x(20);

    int expected_value = 10;
    std::cout << "previous expected value - " << expected_value << std::endl;
    bool return_val = x.compare_exchange_weak(expected_value, 6);

    std::cout << "operation successful    - " << (return_val ? "yes" : "no") << std::endl;
    std::cout << "current expected_value  - " << expected_value << std::endl;
    std::cout << "current x               - " << x.load() << std::endl;
}

int main() {
    run_code1();
}