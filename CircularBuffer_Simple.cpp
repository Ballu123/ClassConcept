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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 8

typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    int head;       // Write index
    int tail;       // Read index
    int count;      // Number of elements
} CircularBuffer;

/*--------------------------------------------------
 Initialize Buffer
---------------------------------------------------*/
void initBuffer(CircularBuffer *cb)
{
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

/*--------------------------------------------------
 Check Empty
---------------------------------------------------*/
bool isEmpty(CircularBuffer *cb)
{
    return (cb->count == 0);
}

/*--------------------------------------------------
 Check Full
---------------------------------------------------*/
bool isFull(CircularBuffer *cb)
{
    return (cb->count == BUFFER_SIZE);
}

/*--------------------------------------------------
 Write Data
 Returns:
   true  -> Success
   false -> Buffer Full
---------------------------------------------------*/
bool writeBuffer(CircularBuffer *cb, uint8_t data)
{
    if (isFull(cb))
    {
        printf("Buffer Overflow!\n");
        return false;
    }
    cb->buffer[cb->head] = data;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    cb->count++;
    return true;
}

/*--------------------------------------------------
 Read Data
 Returns:
   true  -> Success
   false -> Buffer Empty
---------------------------------------------------*/
bool readBuffer(CircularBuffer *cb, uint8_t *data)
{
    if (isEmpty(cb))
    {
        printf("Buffer Underflow!\n");
        return false;
    }
    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    cb->count--;
    return true;
}

/*--------------------------------------------------
 Print Buffer
---------------------------------------------------*/
void printBuffer(CircularBuffer *cb)
{
    printf("\nBuffer Contents:\n");

    if (isEmpty(cb))
    {
        printf("Buffer Empty\n");
        return;
    }
    int index = cb->tail;
    for (int i = 0; i < cb->count; i++)
    {
        printf("%d ", cb->buffer[index]);
        index = (index + 1) % BUFFER_SIZE;
    }
    printf("\n");
}

/*--------------------------------------------------
 Get Current Size
---------------------------------------------------*/
int size(CircularBuffer *cb)
{
    return cb->count;
}

/*--------------------------------------------------
 Main
---------------------------------------------------*/
int main()
{
    CircularBuffer cb;
    uint8_t data;

    initBuffer(&cb);
    printf("Writing...\n");

    for(int i=1;i<=8;i++)
        writeBuffer(&cb,i*10);

    printBuffer(&cb);

    printf("\nTrying Overflow...\n");
    writeBuffer(&cb,99);

    printf("\nReading...\n");

    while(readBuffer(&cb,&data))
    {
        printf("%d ",data);
    }

    printf("\n");

    printf("\nTrying Underflow...\n");
    readBuffer(&cb,&data);

    printf("\nCurrent Size = %d\n",size(&cb));

    return 0;
}
