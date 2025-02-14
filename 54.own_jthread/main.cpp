#include <chrono>
#include <iostream>

#include "jthread.h"

using namespace std::literals;

//void func(int x, int y, std::stop_token token) - the last is token

//interruptable
void do_something() {
    int counter{0};
    while (counter < 10)
    {
        if (interrupt_point()) {
            return;
        }
        std::this_thread::sleep_for(0.2s);
        std::cerr << "This is interruptable thread: " << counter << std::endl;
        ++counter;
    }
    
}

void do_something_else() {
    int counter{0};
    while (counter < 10)
    {
        std::this_thread::sleep_for(0.2s);
        std::cerr << "This is non-interruptable thread: " << counter << std::endl;
        ++counter;
    }
    
}

int main() {
    std::cout << std::endl;
    jthread_local NonInterruptable(do_something_else);
    jthread_local interruptable(do_something);

    std::this_thread::sleep_for(1.0s);
    interruptable.interrupt();
    NonInterruptable.interrupt();
    
    // jthread has its own deconstructor which calls for join function, detach func shall be called manually from main()
    //jthread has interruption function to interrupt execution - important for modern applications
    // conditions checks with using "stop_token" to achieve interruptable
    // we can introduce condition checks with "stop_token" to specify interrupt points\

    return 0;
}