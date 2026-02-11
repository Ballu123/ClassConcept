/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: Thread_PrintABC_seq_Semaphore.c
 * Platform: Linux
 * Semaphores to ensure that Thread A, Thread B, and Thread C print sequentially
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

// Declare semaphores
sem_t semA, semB, semC;

// Number of iterations to print A, B, C
int iterations = 10; // Adjust this if needed

void* threadA(void* arg) {
    while (1) {
        sem_wait(&semA);  // Wait for the signal to start
        
        // Thread A prints its part
        printf("A\n");
        
        sem_post(&semB);  // Signal Thread B to start
    }
    return NULL;
}

void* threadB(void* arg) {
    while (1) {
        sem_wait(&semB);  // Wait for Thread A to finish
        
        // Thread B prints its part
        printf("B\n");
        
        sem_post(&semC);  // Signal Thread C to start
    }
    return NULL;
}

void* threadC(void* arg) {
    while (1) {
        sem_wait(&semC);  // Wait for Thread B to finish
        
        // Thread C prints its part
        printf("C\n");
        
        sem_post(&semA);  // Signal Thread A to start again
    }
    return NULL;
}

int main() {
    pthread_t tA, tB, tC;

    // Initialize semaphores
    // pshared = 0 (threads), value = 1 (one thread at a time) 
    //	0: Shared between threads of the same process. Non-zero: Shared between processes (requires shared memory).
    sem_init(&semA, 0, 1);  // semA starts at 1, allowing Thread A to start
    sem_init(&semB, 0, 0);  // semB starts at 0, blocking Thread B
    sem_init(&semC, 0, 0);  // semC starts at 0, blocking Thread C

    // Create threads
    pthread_create(&tA, NULL, threadA, NULL);
    pthread_create(&tB, NULL, threadB, NULL);
    pthread_create(&tC, NULL, threadC, NULL);

    // The main thread can do other things or simply wait
    // For demonstration purposes, we will sleep for a while
    sleep(10); // Run for 10 seconds

    // After sleep, you may want to terminate the program
    // If you want to terminate it gracefully, you should implement a way to stop the threads

    // Wait for all threads to finish (this will never happen in this case)
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);

    // Destroy semaphores
    sem_destroy(&semA);
    sem_destroy(&semB);
    sem_destroy(&semC);

    return 0;
}
