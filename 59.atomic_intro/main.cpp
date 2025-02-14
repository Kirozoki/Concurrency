/*
Atomic operation - indivisible one
i++;
Normal operation:
Step#1: Read the value "i" from memory
Step#2: Increment that value by one
Step#3: Store the mofified value to the memory
std::atomic<int> or std::atomic_int 

most basic: 
std::atomic_flag - boolean type but initialized by not boolean value like (true/false)
it's to be with ATOMIC_FLAG_INIT 
operations: clear(), test_and_set() 

all other types provide additional one : is_lock_free()
function
*/
#include <iostream>
#include <thread>
#include <atomic>

int main() {
    std::atomic_flag flag1 = true; // it's possible now!!!
    std::atomic_flag flag2 = ATOMIC_FLAG_INIT;

    //set the flag value to true and return previous value
    std::cout << "1.previous flag value  : " << flag2.test_and_set() << std::endl; // 0 previous value printed, changed to true 1
    std::cout << "2.previous flag value  : " << flag2.test_and_set() << std::endl; // 1 printed

    //clear the flag value
    flag2.clear(); // erase true value, with setting to false 
    std::cout << "3.previous flag value  : " << flag2.test_and_set() << std::endl; // 

}