/*
barrier - synchronization mechanism that make threads to wait until 
the required number of threads has reached a certian point in code.
Once all required threads have reached the barrier, they are all unblocked
and may proceed. 
!!!boost is used for extended version of barrier
*/
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/bind/bind.hpp>
#include <boost/atomic.hpp>
#include <iostream>
//compilation via terminal for boost::thread via command: 
//g++ main.cpp -o output -lboost_thread 
void func(boost::barrier& cur_barrier, boost::atomic<int>& counter) {
    ++counter;
    cur_barrier.wait();
    std::cout << "Current value after barrier :" << counter << "\n";
}

int main()
{
    boost::barrier bar(3);
    boost::atomic<int> current(0);
    boost::thread thr1(boost::bind(&func, boost::ref(bar), boost::ref(current)));
    boost::thread thr2(boost::bind(&func, boost::ref(bar), boost::ref(current)));
    boost::thread thr3(boost::bind(&func, boost::ref(bar), boost::ref(current)));
    thr1.join();
    thr2.join();
    thr3.join();
}