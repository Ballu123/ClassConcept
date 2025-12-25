/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: prod_cons.c
 * Win10
 * Microsoft Visual Studio
 * This program demonstrates a Thread safe producer-consumer problem solution using pthreads with proper synchronization to avoid race conditions. 
 * The producer and consumer run indefinitely, producing and consuming items from the shared buffer.
 * cond_producer is a condition variable to signal the producer when there is space in the buffer. 
 * cond_consumer is a condition variable to signal the consumer when there are items in the buffer.
 * Produces items and adds them to the buffer. If the buffer is full, it waits for the consumer to consume an item. 
 * Signals the consumer whenever it adds a new item to the buffer.
 * Consumes items from the buffer. If the buffer is empty, it waits for the producer to produce an item. 
 * Signals the producer whenever it consumes an item from the buffer
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

// Shared buffer
int buffer[BUFFER_SIZE];
int count = 0;  // Number of items in the buffer

// Mutex and condition variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

void* producer(void* arg) {
    int item;

    while (1) {
        item = rand() % 100;  // Produce a random item
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            // Buffer is full, wait for the consumer to consume
            pthread_cond_wait(&cond_producer, &mutex);
        }

        // Add the item to the buffer
        buffer[count] = item;
        count++;
        printf("Producer produced: %d\n", item);

        // Signal the consumer that an item is available
        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);

        sleep(1);  // Simulate time taken to produce an item
    }

    return NULL;
}

void* consumer(void* arg) {
    int item;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            // Buffer is empty, wait for the producer to produce
            pthread_cond_wait(&cond_consumer, &mutex);
        }

        // Remove the item from the buffer
        item = buffer[--count];
        printf("Consumer consumed: %d\n", item);

        // Signal the producer that there is space available
        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);

        sleep(1);  // Simulate time taken to consume an item
    }

    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Create the producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for the threads to finish (they won't in this infinite loop example)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy mutex and condition variables (unreachable in this example)
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_producer);
    pthread_cond_destroy(&cond_consumer);

    return 0;
}
