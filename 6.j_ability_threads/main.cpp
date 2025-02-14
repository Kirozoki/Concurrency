#include <iostream>
#include <thread>

void test() {
    printf(" Hello from test \n");
}

void run() {
    std::thread thread1(test);

    if (thread1.joinable()) {
        printf("Thread 1 is joinable! \n");
    } else {
        printf("Thread 1 is non joinable! \n");
    }

    thread1.join();

    if (thread1.joinable()) {
        printf("Thread 1 is joinable! \n");
    } else {
        printf("Thread 1 is non joinable! \n");
    }

    std::thread thread2;

    if (thread2.joinable()) {
        printf("Thread 2 is joinable! \n");
    } else {
        printf("Thread 2 is non joinable! \n");
    }

}

int main() {
    run();
}