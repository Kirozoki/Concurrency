#include <iostream>
#include <thread>

using namespace std::literals;

void foo() {
    std::cout << "hello from foo" << std::endl;
}

void bar() {
    std::cout << "hello from bar" << std::endl;
}

int main() {

    std::thread thread1(foo);

    std::thread thread2 = std::move(thread1);

    thread1 = std::thread(bar);
    std::thread thread3(foo);

    thread1 = std::move(thread3);

    thread1.join();
    thread2.join();
    thread3.join();
}