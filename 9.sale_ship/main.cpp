#include <iostream>
#include <thread>

#include "common.h"

enum struct Command{NoCommand, CleanCommand, AheadCommand, StopCommand};

void cleaning() {
    printf("It's cleaining \n");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    printf("Cleaning is over!");
}

void speed_ahead() {
    printf("It's speed ahead command \n");    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    printf("Speed has been achieved!");
}

void stop() {
    printf("It's stop command \n");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    printf("The engine has been stopped!");
    
}

int main() { 
    int order_numb;
    Command command;
    while (std::cin >> order_numb) {
        switch (order_numb) {
        case 1: {
            // Cleaning, do not need to wait for ending the command - detach
            std::thread cleaning_thread(cleaning);
            cleaning_thread.detach();
        break;}
        case 2: {
            // full spead ahead - join
            std::thread speed_thread(speed_ahead);
            speed_thread.join();
        break;}
        case 3: {
            // stop engine - join
            std::thread stop_thread(stop);
            stop_thread.join();
        break;}
        case 100: 
            // exit the programm
            return 0;
        break;
        default:
            printf("Invalid order \n");
        break;
        }
    }
}