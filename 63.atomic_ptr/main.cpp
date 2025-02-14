/*
atomic_ptr make ptr as atomic. but contained variable is not necessary atomic. 
other atomic rules are the same. 
fucntions: 
-is_lock_free()
-store()
-load()
-exchange()
-compare_exchange_weak(.., ..)
-compare_exchange_strong(.., ..)

Extra: 
-fetch_add, +=
-fetch_sub, -=
- ++
- --
*/

#include <iostream>
#include <thread>
#include <atomic>

void run_code1() {
    int values[20];

    for (int i = 0; i < 20; ++i) {
        values[i] = i + 1;
    }

    std::atomic<int*> x_pointer = values;
    std::cout << "atomic integer pointer lock free ? " 
                << (x_pointer.is_lock_free() ? "yes" : "no")
                << std::endl;
    int* y_pointer = values + 3;

    x_pointer.store(y_pointer);
    std::cout << "value referening to by pointer : " << *(x_pointer.load()) << std::endl;

    bool ret_val = x_pointer.compare_exchange_weak(y_pointer, values + 10);
    std::cout << "store operation successfull : " << (ret_val ? "yes" : "no") << std::endl;
    std::cout << "new value pointer by atomic pointer : " << *x_pointer << std::endl;
}

void run_code2() {
    int values[20];

    for (int i = 0; i < 20; ++i) {
        values[i] = i + 1;
    }

    std::atomic<int*> x_pointer = values;
    std::cout << "1. After initializing value pointed by atomic pointer       - " 
                << *x_pointer
                << std::endl;

    //fetch add
    int* prev_pointer_val_1 = x_pointer.fetch_add(12);

    std::cout << "2. After fetch add previous value pointed by atomic pointer - " 
                << *prev_pointer_val_1 << std::endl;
    std::cout << "2. After fetch add new value pointed by atomic pointer - " 
                << *x_pointer << std::endl;

    //fetch sub
    int* prev_pointer_val_2 = x_pointer.fetch_sub(3);

    std::cout << "3. After fetch sub previous value pointed by atomic pointer - " 
                << *prev_pointer_val_2 << std::endl;
    std::cout << "3. After fetch sub new value pointed by atomic pointer - " 
                << *x_pointer << std::endl;

    //++ operator
    x_pointer++;
    std::cout << "4. After post increment value pointed by atomic pointer - " 
                << *x_pointer << std::endl;

    //-- operator
    x_pointer--;
    std::cout << "5. After post decrement value pointed by atomic pointer - " 
                << *x_pointer << std::endl;
}

int main() {
    run_code2();
}

