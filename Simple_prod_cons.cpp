/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: Simple_prod_cons.cpp
  */
// solved using conditional variables
// THE PROBLEM STATEMENT:
//  1. The producer will produce and the consumer will consume with synchronization of a common buffer.
//  2. Until producer thread produces any data consumer thread can't consume.
//  3. Threads will use condition_variable to notify each other.
//  4. We need mutex if we use condition_variable because CV waits on mutex.
//  5. This is one of the example of producer consumer there are many.

// PRODUCER thread steps:
//  1. lock mutex, if success then go ahead otherwise wait for mutex to get free.
//  2. Check if buffer is full and if it is full then unlock mutex and sleep, if not then go ahead and produce.
//  3. Insert item in buffer.
//  4. unlock mutex. 
//  5. notify consumer.

// CONSUMER thread steps:
//  1. lock mutex, if success then go ahead otherwise wait for mutex to get free.
//  2. check if buffer is empty and if it is, then unlock the mutex and sleep, if not thean go ahead and consume.
//  3. consume item from buffer.
//  4. unlock mutex.
//  5. notify producer.

// handles all the cases below
// case 1 consumer can start first even if there is no data to consume
// case 2 producer does not produce more data than the buffer can store

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <bits/stdc++.h>

using namespace std;

std::mutex m;
std::condition_variable cv;
deque<int> buffer;
const unsigned int maxSize = 50;

void producer(int data) {
    while (data) {
        std::unique_lock<std::mutex> mylock(m);         // I got to know that lock_guard cannot be used with conditional variables
        cv.wait(mylock, []() {return buffer.size() < maxSize;});
        buffer.push_back(data);
        std::cout << "Producer produced: " << data-- << std::endl;
        mylock.unlock();
        cv.notify_one();
    }
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> mylock(m);
        cv.wait(mylock, []() {return buffer.size() > 0;});
        int val = buffer.back();
        buffer.pop_back();
        std::cout << "Consumer consumed: " << val << std::endl;
        mylock.unlock();
        cv.notify_one();
    }
}


int main() {
    std::thread t1(consumer);
    std::thread t2(producer, 100);

    t1.join();
    t2.join();

    return 0;
}
