#include <coroutine>
#include <iostream>
#include <vector>

template<typename T>
struct Generator
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;

    Generator(handle_type h) : coro(h) {}                         // (3)
    

    ~Generator() {
        if (coro) coro.destroy();
    }
    Generator(const Generator&) = delete;
    Generator& operator = (const Generator&) = delete;
    Generator(Generator&& oth) noexcept : coro(oth.coro) {
        oth.coro = nullptr;
    }
    Generator& operator = (Generator&& oth) noexcept {
        coro = oth.coro;
        oth.coro = nullptr;
        return *this;
    }
    T getValue() {
        return coro.promise().current_value;
    }
    bool next() {                                                // (5)
        coro.resume();
        return not coro.done();
    }
};

template<typename T>
struct Generator<T>::promise_type {
        using coro_handle = std::coroutine_handle<promise_type>;
        
        auto get_return_object() {                               // (2)
            return coro_handle::from_promise(*this);
        }

        auto initial_suspend() {                                 // (4)
            return std::suspend_always();
        }
        auto  final_suspend() noexcept {
            return std::suspend_always();
        }
        
        void return_void() {}

        auto yield_value(const T value) {                        // (6) 
            current_value = value;
            return std::suspend_always{};
        }
        void unhandled_exception() {
            std::terminate();
        }

        T current_value;
    };

Generator<int> getNext(int start = 0, int step = 1) noexcept {
    auto value = start;
    for (int i = 0;; ++i) { // there's no conditional limit as it's guided by coroutine
        co_yield value;
        value += step;
    }
}

/*std::vector<int> getNumbers(int begin, int end, int inc = 1) {
    std::vector<int> numbers;
    for(int i = begin; i <= end; i += inc) {
        numbers.push_back(i);
    }
    return numbers;
}*/

int main() {

/*std::cout << std::endl;

    std::cout << "getNumbers():";
    auto gen = getNumbers(1, 1000);
    for (int i = 0; i < 100; ++i) {
        std::cout << gen[i] << " ";                      
    }

    std::cout << "\n\n";

    for (int i = 100; i < 1000; ++i) {
        std::cout << gen[i] << " "; 
    }

std::cout << std::endl;*/

std::cout << std::endl;

    std::cout << "getNext():";
    auto gen1 = getNext();
    for (int i = 0; i <= 10; ++i) {
        gen1.next();
        std::cout << " " << gen1.getValue();                      // (7)
    }

    std::cout << "\n\n";

    std::cout << "getNext(100, -10):";
    auto gen2 = getNext(100, -10);
    for (int i = 0; i <= 20; ++i) {
        gen2.next();
        std::cout << " " << gen2.getValue();
    }

std::cout << std::endl;

}