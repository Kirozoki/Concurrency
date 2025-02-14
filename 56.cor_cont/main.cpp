/*
in order to use :
*co_yield
*co_await
*co_return

we need to make an effort with special class for it!
*/

/*
Coroutine consist of :
- a promise object:
    * User defined promise_type object
    * Manipulate inside coroutine
    * return result via this object
- handle
    * Non owning handle which use to resume or destroy coroutine from outside 
- coroutine state
    * Heap allocated
    * Contains promise object, arguments to coroutine and local variables
*/

#include <coroutine>
#include <iostream>
#include <cassert>

class resumable {
    public:
    struct promise_type;
    using coro_handle = std::coroutine_handle<promise_type>;
    resumable(coro_handle handle) : handle_(handle) {assert(handle);}
    resumable(resumable&) = delete;
    resumable(resumable&&) = delete;
    ~resumable() {handle_.destroy();}
    bool resume() {
        if (not handle_.done()) {
            handle_.resume();
        }
        return not handle_.done();
    }
    private:
    coro_handle handle_;
};

struct resumable::promise_type
{
    using coro_handle = std::coroutine_handle<promise_type>;
    auto get_return_object() noexcept {
        return coro_handle::from_promise(*this);
    }
    auto initial_suspend() noexcept {return std::suspend_always();}
    auto final_suspend() noexcept {return std::suspend_always();}
    void return_void() noexcept {}
    void unhandled_exception() noexcept {
        std::terminate();
    }
};


resumable foo() {
    std::cout << "a" << std::endl;
    co_await std::suspend_always();
    std::cout << "b" << std::endl;
    co_await std::suspend_always();
    std::cout << "c" << std::endl;
}

int main() {
    resumable res = foo();
    res.resume();
    res.resume();
    res.resume();
}

