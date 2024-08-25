/*
 * Name:Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: CircularBuffer.cpp
 * Win10
 * Microsoft Visual Studio
 * ref : https://embedjournal.com/implementing-circular-buffer-embedded-c/
 * ref : http://www.java2s.com/Tutorial/Java/0140__Collections/CircularBuffer.htm
 * ref : https://gist.github.com/xstherrera1987/3196485
 * Circular buffer is a FIFO data structure that treats memory to be circular; that is, the read/write indices loop back to 0 after it reaches the buffer length. 
 * This is achieved by two pointers to the array, the “head” pointer and the “tail” pointer. As data is added (write) to the buffer, the head pointer is incremented and likewise, 
 * when the data is being removed (read) the tail pointer is incremented. The definition of head, tail, their movement direction and write and read location are all implementation dependent but the idea/goal remains the same.
 */

#include <iostream>
#include <vector>

template <typename T>
class CircularBuffer {
private:
    std::vector<T> buffer;
    size_t capacity;
    size_t readIndex;
    size_t writeIndex;
    bool full;

public:
    CircularBuffer(size_t size) : buffer(size), capacity(size), readIndex(0), writeIndex(0), full(false) {}

    bool isEmpty() const {
        return !full && (readIndex == writeIndex);
    }

    bool isFull() const {
        return full;
    }

    size_t size() const {
        if (full) {
            return capacity;
        } else if (writeIndex >= readIndex) {
            return writeIndex - readIndex;
        } else {
            return capacity - (readIndex - writeIndex);
        }
    }

    bool enqueue(const T& item) {
        if (full) {
            return false; // Buffer is full, cannot enqueue
        }

        buffer[writeIndex] = item;
        writeIndex = (writeIndex + 1) % capacity;

        if (writeIndex == readIndex) {
            full = true;
        }

        return true;
    }

    bool dequeue(T& item) {
        if (isEmpty()) {
            return false; // Buffer is empty, cannot dequeue
        }

        item = buffer[readIndex];
        readIndex = (readIndex + 1) % capacity;
        full = false;

        return true;
    }
};

int main() {
    CircularBuffer<int> buffer(5);

    buffer.enqueue(10);
    buffer.enqueue(20);
    buffer.enqueue(30);

    int item;
    while (buffer.dequeue(item)) {
        std::cout << "Dequeued: " << item << std::endl;
    }

    return 0;
}
