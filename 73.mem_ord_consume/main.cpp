#include <iostream>
#include <atomic>
#include <thread>
#include <assert.h>

struct X{
    int i;
    std::string s;
};

std::atomic<X*> ptr;
std::atomic<int> a;

void create_x() {
    X* x = new X;
    x->i = 42;
    x->s = "hello";

    a.store(20, std::memory_order_relaxed); // maybe lost
    ptr.store(x, std::memory_order_release); // direct dependecy
}

void use_x() {
    X* x;
    while(!(x = ptr.load(std::memory_order_consume))); // limit to direct dependency only
    assert(x->i == 42);
    assert(x->s == "hello");
    assert(a.load(std::memory_order_relaxed) == 20); // might faile as there's no guarantee that 'a' sees the value
}

void run_code() {
    std::thread thread_create(create_x);
    std::thread thread_use(use_x);

    thread_create.join();
    thread_use.join();
}

int main() {
    run_code();
}