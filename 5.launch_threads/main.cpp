#include <iostream>
#include <thread>

void functionTest() {
    printf("Hello from test \n");
}

void functionA() {
    printf("Hello from function A \n");
    
    std::thread threadC(functionTest);
    threadC.join();
    
}

void functionB() {
    printf("Hello from function B \n");
}

void run() {
    std::thread threadA(functionA);
    
    std::thread threadB(functionB);

    threadB.join();
    threadA.join();

    printf("Hello from main \n");
}

int main() {
    run();
}