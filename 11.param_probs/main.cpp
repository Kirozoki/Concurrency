#include <iostream>
#include <thread>

using namespace std::literals;

void func2(int& x) {
    while (true)
    {
        try {

        std::cout << x << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        } catch (...) {
            throw std::runtime_error("this is runtime error"); // after thread1 finished it is calling non-exist variable
        }
    }
    
}

void func1() {
    int x = 5;
    std::thread thread2(func2, std::ref(x));
    thread2.detach();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "thread1 finished execution \n";
}

int main() {

    std::thread thread1(func1);
    thread1.join();

}