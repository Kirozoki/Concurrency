#include <iostream>
#include <thread>

using namespace std::literals;

void func1(int x, int y){
    printf("X + Y = %d\n", x + y);
}

void func2(int& x) {
    while (true)
    {
        printf("Print 1 value of X - %d\n", x);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
}

int main() {
    /*int p = 9;
    int q = 8;

    std::thread sum_thread(func1, p, q);

    sum_thread.join();
*/
    int x = 9;

    printf("Main thread value X - %d\n", x);
    
    std::thread val_thread(func2, std::ref(x));

    std::this_thread::sleep_for(std::chrono::seconds(5));

    x= 15;
    printf("Main thread value X has been changed - %d\n", x);

    val_thread.join();

}