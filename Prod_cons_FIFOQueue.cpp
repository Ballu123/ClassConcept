/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: FIFOQueue_prod_cons.cpp
 * Write a class in C++ to implement a First-in-first-out queue for passing data between producer and consumer threads. 
 * Requirement: You can only allocate one fixed-size memory space for the implementation. The class has the following member functions:
 * bool push(int value); // add data to FIFO, return false if the queue is full int pop(); // get data from FIFO
 * FIFOQueue class with proper synchronization mechanisms using std::mutex and std::condition_variable to ensure thread safety in a multithreaded environment.
 * In this version, proper mutex locking and unlocking, along with condition variable notifications, 
 * This implementation ensures that the producer and consumer threads work together correctly synchronized. 
 * This implementation prevents data races and guarantees safe access to the shared resources of the FIFO queue.
 * atul: https://github.com/atulraut/atclib/blob/master/thread/crack-d-ivi/producerconsumer.c
 */

#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>

class FIFOQueue {
private:
    std::vector<int> data;
    size_t capacity;
    size_t front;
    size_t rear;
    size_t count;

    std::mutex mtx;
    std::condition_variable notFull;
    std::condition_variable notEmpty;

public:
    FIFOQueue(size_t size) : capacity(size), front(0), rear(0), count(0) {
        data.resize(capacity);
    }

    bool push(int value) {
        std::unique_lock<std::mutex> lock(mtx);
        notFull.wait(lock, [this]() { return count < capacity; });

        data[rear] = value;
        rear = (rear + 1) % capacity;
        count++;

        notEmpty.notify_one();
        return true;
    }

    int pop() {
        std::unique_lock<std::mutex> lock(mtx);
        notEmpty.wait(lock, [this]() { return count > 0; });

        int value = data[front];
        front = (front + 1) % capacity;
        count--;

        notFull.notify_one();
        return value;
    }
};

int main() {
    FIFOQueue queue(5); // Create a FIFO queue with a capacity of 5

    // Producer thread
    std::thread producer([&]() {
        for (int i = 0; i < 10; ++i) {
            queue.push(i);
            std::cout << "Produced: " << i << std::endl;
        }
    });

    // Consumer thread
    std::thread consumer([&]() {
        for (int i = 0; i < 10; ++i) {
            int value = queue.pop();
            std::cout << "Consumed: " << value << std::endl;
        }
    });

    producer.join();
    consumer.join();

    return 0;
}

=====** 2nd way **====
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

class BoundedBuffer {
private:
    std::vector<int> buffer;
    int capacity;
    int front = 0;
    int rear = 0;
    int count = 0;

    std::mutex mtx;
    std::condition_variable notFull;  // Signaled when a slot becomes free
    std::condition_variable notEmpty; // Signaled when data is added

public:
    explicit BoundedBuffer(int cap) : buffer(cap), capacity(cap) {}

    void produce(int item) {
        std::unique_lock<std::mutex> lock(mtx);

        // 1. Wait if full: Releases lock and sleeps until a consumer takes an item
        notFull.wait(lock, [this]() { return count < capacity; });

        // 2. Add item to circular buffer
        buffer[rear] = item;
        rear = (rear + 1) % capacity;
        count++;

        std::cout << "[Producer] Added: " << item << " | Buffer Size: " << count << std::endl;

        // 3. Notify a waiting consumer that the buffer is no longer empty
        notEmpty.notify_one();
    } // lock is released here

    int consume() {
        std::unique_lock<std::mutex> lock(mtx);

        // 1. Wait if empty: Releases lock and sleeps until a producer adds an item
        notEmpty.wait(lock, [this]() { return count > 0; });

        // 2. Remove item from circular buffer
        int item = buffer[front];
        front = (front + 1) % capacity;
        count--;

        std::cout << "[Consumer] Took: " << item << " | Buffer Size: " << count << std::endl;

        // 3. Notify a waiting producer that space is now available
        notFull.notify_one();
        
        return item;
    } // lock is released here
};

void producerTask(BoundedBuffer& buf) {
    for (int i = 1; i <= 10; ++i) {
        buf.produce(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumerTask(BoundedBuffer& buf) {
    for (int i = 1; i <= 10; ++i) {
        buf.consume();
        std::this_thread::sleep_for(std::chrono::milliseconds(250)); // Slower consumer
    }
}

int main() {
    BoundedBuffer buffer(3); // Small capacity to force threads to wait

    std::thread t1(producerTask, std::ref(buffer));
    std::thread t2(consumerTask, std::ref(buffer));

    t1.join();
    t2.join();

    return 0;
}

