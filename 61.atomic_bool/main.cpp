/*
atomic<*> : 
- neither copy assignable nor copy constructable
- can assign non atomic Booleans and can be constructed using non atomic Booleans

functions on atomic_bool
-is_lock_free
-store
-load
-exchange (replace the stored value with a new one and automatically retrieve the original one)
- compare_exchange_weak
- compare_exchange_strong

functions above are the same for other atomic flags
*/

#include <iostream>
#include <thread>
#include <atomic>

void run_code1() {
    std::atomic<bool> flag1; // assigned as false by default
    std::cout << "flag 1 = " << flag1 << std::endl;
    //cannot copy construct
    //std::atomic<bool> flag2(flag1);

    //cannot copy assignable
    //std::atomic<bool> flag3 = flag1;

    //construct using non atomic boolean value
    bool non_atomic_bool = true;
    std::atomic<bool> flag4(non_atomic_bool);
    std::cout << "flag 4 = " << flag4 << std::endl;

    //construct using non atomic boolean value
    std::atomic<bool> flag5 = non_atomic_bool;
    std::cout << "flag 5 = " << flag5 << std::endl;
}

void run_code2() {
    std::atomic<bool> x(false);
    std::cout << "atomic boolean is implemented lock free - "
            << (x.is_lock_free() ? "yes" : "no") << std::endl;
    
    std::atomic<bool> y(true);

    //store operations
    x.store(false);
    x.store(y);

    //load operations
    std::cout << "value of the atomic bool y - " << y.load() << std::endl;

    //exchange operation
    bool z = x.exchange(false);

    std::cout << "current value of atomic bool x - " << x.load() << std::endl;
    std::cout << "previous value of atomic bool x - " << z << std::endl;
}

int main() {
    run_code2();
}