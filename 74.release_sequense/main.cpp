#include <atomic>
#include <queue>
#include <chrono>
#include <thread>

std::atomic<int> count;
std::queue<int> data_queue;
int max_count = 5;

void writer_queue() {
    for (int i = 0; i < 5; ++i) {
        data_queue.push(i);
    }

    count.store(5, std::memory_order_release);
}

void reader_queue() {
    while(true) {
        int item_index = 0;
        if (!(item_index = count.fetch_sub(1, std::memory_order_acquire) <= 0)) {
            //wait for items
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }
        //process the item
    }
}

void run_code() {
    std::thread writer_thread(writer_queue);
    std::thread reader_thread1(reader_queue);
    std::thread reader_thread2(reader_queue);

    writer_thread.join();
    reader_thread1.join();
    reader_thread2.join();
}

int main() {
    run_code();
}

// thread1 and thread2 affects atomic value. 
//writer may not be linked with the last thread
//UNCLEAR!
