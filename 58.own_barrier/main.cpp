#include <thread>
#include <atomic>
#include <condition_variable>
#include <iostream>

class barrier_spin
{
private:
    unsigned const thres_hold;
    std::atomic<unsigned> count;
    std::atomic<unsigned> generation;

public:
    explicit barrier_spin(unsigned _thres_hold) :
    thres_hold(_thres_hold), count(_thres_hold), generation(0)
    {
    }

    void wait() {
        unsigned const my_generation = generation;

        if (!--count) {
            count = thres_hold;
            ++generation;
        } else {
            while (generation == my_generation) {
                std::this_thread::yield();
            }
        }
    }

};

class barrier_cond
{
private:
std::mutex mMutex;
std::condition_variable mCond;
std::size_t mThreshold;
std::size_t mCount;
std::size_t mGeneration;
    
public:
    explicit barrier_cond(std::size_t iCount) :
    mThreshold(iCount),
    mCount(iCount),
    mGeneration(0) {}

    void Wait() {
        std::unique_lock<std::mutex> lLock{mMutex};
        auto lGen = mGeneration;
        if (!--mCount) {
            mGeneration++;
            mCount = mThreshold;
            mCond.notify_all();
        } else {
            mCond.wait(lLock, [this, lGen] {return lGen != mGeneration;});
        }
    }
    
};

void func(barrier_cond& cur_barier, std::atomic<int>& counter)
{
    ++counter;
    //std::cout  << counter << "\n";
    cur_barier.Wait();
    std::cout << "Current value after barrier :" << counter << "\n";
}

int main()
{
    barrier_cond bar(3);
    std::atomic<int> current(0);
    std::thread thr1(&func, std::ref(bar), std::ref(current));
    std::thread thr2(&func, std::ref(bar), std::ref(current));
    std::thread thr3(&func, std::ref(bar), std::ref(current));
    std::thread thr4(&func, std::ref(bar), std::ref(current));
    std::thread thr5(&func, std::ref(bar), std::ref(current));
    std::thread thr6(&func, std::ref(bar), std::ref(current));
    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    thr5.join();
    thr6.join();
}

 