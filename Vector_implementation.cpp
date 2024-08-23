

#include <iostream>
#include <stdexcept>

template <typename T>
class Vector {
private:
    T* arr;           // Pointer to the first element of the dynamic array
    size_t _size;     // Number of elements in the vector
    size_t _capacity; // Allocated memory (in terms of the number of elements)

    // Method to resize the array when the capacity is exceeded
    void resize(size_t new_capacity) {
        T* new_arr = new T[new_capacity];
        for (size_t i = 0; i < _size; ++i) {
            new_arr[i] = std::move(arr[i]);
        }
        delete[] arr;
        arr = new_arr;
        _capacity = new_capacity;
    }

public:
    // Constructor
    Vector() : arr(nullptr), _size(0), _capacity(0) {}

    // Destructor
    ~Vector() {
        delete[] arr;
    }

    // Size of the vector
    size_t size() const {
        return _size;
    }

    // Capacity of the vector
    size_t capacity() const {
        return _capacity;
    }

    // Check if the vector is empty
    bool empty() const {
        return _size == 0;
    }

    // Access element at a specific index
    T& operator[](size_t index) {
        if (index >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return arr[index];
    }

    const T& operator[](size_t index) const {
        if (index >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return arr[index];
    }

    // Add an element to the end of the vector
    void push_back(const T& value) {
        if (_size == _capacity) {
            size_t new_capacity = (_capacity == 0) ? 1 : _capacity * 2;
            resize(new_capacity);
        }
        arr[_size++] = value;
    }

    // Remove the last element from the vector
    void pop_back() {
        if (_size > 0) {
            --_size;
        }
    }

    // Access the last element of the vector
    T& back() {
        if (_size == 0) {
            throw std::out_of_range("Vector is empty");
        }
        return arr[_size - 1];
    }

    const T& back() const {
        if (_size == 0) {
            throw std::out_of_range("Vector is empty");
        }
        return arr[_size - 1];
    }

    // Clear the vector
    void clear() {
        _size = 0;
    }
};

int main() {
    Vector<int> vec;

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    std::cout << "Vector size: " << vec.size() << std::endl;
    std::cout << "Vector capacity: " << vec.capacity() << std::endl;

    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << "Element at index " << i << ": " << vec[i] << std::endl;
    }

    vec.pop_back();
    std::cout << "After pop_back, vector size: " << vec.size() << std::endl;
    std::cout << "Last element: " << vec.back() << std::endl;

    return 0;
}
