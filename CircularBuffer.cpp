/*
 * Name:Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: CircularBuffer.cpp
 * Win10
 * Microsoft Visual Studio
 * ref : http://www.equestionanswers.com/c/c-circular-buffer.php
 * ref : https://asawicki.info/news_1468_circular_buffer_of_raw_binary_data_in_c.html
 * Circular buffer program is implementing following functions:
 * initialize Circular Buffer claass ==>queue_t *init_queue(int max_len);
 * size_t write (const char *data, size_t bytes) == int add_buffer(queue_t *q, char *data, int len);
 * size_t read (char *data, size_t bytes) == int get_buffer(queue_t *q, char *data, int len);
 * void printbuff ();
 */
 

#include <algorithm>
#include <iostream>
#include<malloc.h>
#include<memory.h>
#include<stdio.h>


using namespace std;

class CircularBuffer
{
public:
    CircularBuffer (size_t capacity);
    ~CircularBuffer ();
    
    size_t size () const
    {
        return size_;
    }
    size_t capacity () const
    {
        return capacity_;
    }
    // Return number of bytes written.
    size_t write (const char *data, size_t bytes);
    
    // Return number of bytes read.
    size_t read (char *data, size_t bytes);
    
    // print the CircularBuffer
    void printbuff ();

private:
    size_t beg_index_, end_index_, size_, capacity_;
    char *data_;
};

CircularBuffer::CircularBuffer (size_t capacity):beg_index_ (0), end_index_ (0), size_ (0),
capacity_ (capacity)
{
    data_ = new char[capacity];
}

CircularBuffer::~CircularBuffer ()
{
    delete[]data_;
}

size_t CircularBuffer::write (const char *data, size_t bytes) {
    if (bytes == 0)
    return 0;

    size_t capacity = capacity_;
    size_t bytes_to_write = std::min (bytes, capacity - size_);
    cout << " bytes_to_write: " << bytes << endl;   
    // Write in a single step
    if (bytes_to_write <= capacity - end_index_) {
        cout << "===Single step Write data:==="  << endl;
        memcpy (data_ + end_index_, data, bytes_to_write);
        end_index_ += bytes_to_write;

        if (end_index_ == capacity)
            end_index_ = 0;
    }
    // Write in two steps
    else {
        cout << "===2 step Write data:==="  << endl;
        size_t size_1 = capacity - end_index_;
        memcpy (data_ + end_index_, data, size_1);
        size_t size_2 = bytes_to_write - size_1;
        memcpy (data_, data + size_1, size_2);
        end_index_ = size_2;
    }
    
    size_ += bytes_to_write;
    return bytes_to_write;
}

size_t CircularBuffer::read (char *data, size_t bytes) {
    if (bytes == 0)
    return 0;
    
    size_t capacity = capacity_;
    size_t bytes_to_read = std::min (bytes, size_);
    
    // Read in a single step
    if (bytes_to_read <= capacity - beg_index_) {
        cout << "===Single step read data:==="  << endl;
        memcpy (data, data_ + beg_index_, bytes_to_read);
        beg_index_ += bytes_to_read;
        if (beg_index_ == capacity)
        beg_index_ = 0;
    }
    // Read in two steps
    else {
        cout << "===2 step read data:==="  << endl;
        size_t size_1 = capacity - beg_index_;
        memcpy (data, data_ + beg_index_, size_1);
        size_t size_2 = bytes_to_read - size_1;
        memcpy (data + size_1, data_, size_2);
        beg_index_ = size_2;
        //debugging
        /*for (int i = 0; i < capacity_; i++) {
            cout << "read data: " << data[i] << endl;
        }*/
    }
    
    size_ -= bytes_to_read;
    return bytes_to_read;
}

void CircularBuffer::printbuff () {
    size_t start = beg_index_;
    size_t end = end_index_ ;
    int i, count = 0;
    //debugging
    cout << " Inside print size_ : " << size_ << " start : " << start << "  end: " << end << endl;
    for (i = start; count < size_; i = (i + 1) % capacity_) {
        printf (" Elem[%d] = %c\n", i, data_[i]);
        count++;
        if (i == (end -1 )) {
            cout << " printing break" << endl;
            break;
        }
    }
}

int main () {
	CircularBuffer *cb = new CircularBuffer (6);
	//write
	int wirteval1 = cb->write ("abc", 3); // add "abc"
	int wirteval2 = cb->write ("de", 2); // add "de"
	cb->printbuff ();

	//read 
	char *data = (char *) malloc (sizeof (char) * 3);
	memset (data, 0, sizeof (data));
	int readval1 = cb->read (data, 3); // read 3 char --> "abc"
	cout << "readval1:= " << readval1 << endl;
	cb->printbuff ();

	//write
	int wirteval3 = cb->write ("gh", 2); // add "abc"
	cb->printbuff ();

	//read 
	char *data2 = (char *) malloc (sizeof (char) * 5);
	memset (data2, 0, sizeof (data2));
	int readval2 = cb->read (data2, 5); // read 3 char --> "abc"
	cout << "readval2:= " << readval2 << endl;
	cb->printbuff ();

	return 0;
}

/*
[-|-|-|-|-|-]
add "abc"
[a|b|c|-|-|-]
add "de"
[a|b|c|d|e|-]
get 3 chars
[-|-|-|d|e|-] return "abc"
add "ab"
[b|-|-|d|e|a]
get 5 chars
[-|-|-|-|-|-] return "deab", 4
*/
