#include <chrono>
#include <iostream>
#include <thread>

void foo() {
    std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // ms
    printf("Hello from foo \n");
}

void other_operations() {
    printf("This is other operations \n");
    throw std::runtime_error("this is a runtime error");
}

void run() {
    std::thread foo_thread(foo);

    try
    {
        other_operations();
        foo_thread.join();
    }
    catch(...)
    {
        foo_thread.join();
    }
    
}

int main() {
    run();
}