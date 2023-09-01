/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: ScheduleFunctionCall_without_async.cpp
 * If you want to achieve non-blocking behavior without using std::async, 
 * you can manage the scheduling and execution of tasks using a separate thread. Here's an example of using a worker thread to execute scheduled functions:
 * In this example, the Scheduler class manages a worker thread that processes tasks from a queue. 
 * The scheduleFunctionCall method adds tasks to the queue along with their desired delay.
 * The worker thread continuously checks the queue, sleeps for the specified delay, and executes the tasks.
 * This approach provides non-blocking behavior without using std::async, but it does require more manual management of threads, synchronization, and resources.
 */

#include <iostream>
#include <thread>
#include <functional>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>

class Scheduler {
private:
    std::thread workerThread;
    std::queue<std::pair<std::function<void()>, int>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable queueCondition;
    bool running;

public:
    Scheduler() : running(true) {
        workerThread = std::thread(&Scheduler::workerLoop, this);
    }

    ~Scheduler() {
        running = false;
        queueCondition.notify_all();
        workerThread.join();
    }

    void scheduleFunctionCall(std::function<void()> function, int delayMs) {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskQueue.push(std::make_pair(function, delayMs));
        queueCondition.notify_one();
    }

private:
    void workerLoop() {
        while (running) {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (taskQueue.empty()) {
                queueCondition.wait(lock);
            } else {
                auto task = taskQueue.front();
                taskQueue.pop();
                lock.unlock();

                std::this_thread::sleep_for(std::chrono::milliseconds(task.second));
                task.first();
            }
        }
    }
};

void printMessage() {
    std::cout << "Function call executed!" << std::endl;
}

int main() {
    Scheduler scheduler;
    
    std::cout << "Scheduling function call..." << std::endl;
    scheduler.scheduleFunctionCall(printMessage, 2000); // Call after 2 seconds
    
    // Do other tasks
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    return 0;
}
