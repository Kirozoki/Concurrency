#include <iostream>
#include <thread>

#include "common.h"

void foo() {
    printf("Hello from foo \n");
}

void other_operations() {
    printf("This is other operations \n");
    throw std::runtime_error("this is a runtime error");
}

void run() {
    std::thread foo_thread(foo);

    thread_guard tg(foo_thread);
    
    try
    {
        other_operations();
    }
    catch(...)
    {
    }
    
    
}

int main() {
    run();
}