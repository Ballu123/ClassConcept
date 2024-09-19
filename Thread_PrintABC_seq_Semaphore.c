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

void* threadA(void* arg) {
    sem_wait(&semA);  // Wait for the signal to start (initially unlocked)
    
    // Thread A prints its part
    printf("Thread A\n");
    
    sem_post(&semB);  // Signal Thread B to start
    return NULL;
}

void* threadB(void* arg) {
    sem_wait(&semB);  // Wait for Thread A to finish
    
    // Thread B prints its part
    printf("Thread B\n");
    
    sem_post(&semC);  // Signal Thread C to start
    return NULL;
}

void* threadC(void* arg) {
    sem_wait(&semC);  // Wait for Thread B to finish
    
    // Thread C prints its part
    printf("Thread C\n");
    
    return NULL;
}

int main() {
    pthread_t tA, tB, tC;

    // Initialize semaphores
    sem_init(&semA, 0, 1);  // semA starts at 1, allowing Thread A to start
    sem_init(&semB, 0, 0);  // semB starts at 0, blocking Thread B
    sem_init(&semC, 0, 0);  // semC starts at 0, blocking Thread C

    // Create threads
    pthread_create(&tA, NULL, threadA, NULL);
    pthread_create(&tB, NULL, threadB, NULL);
    pthread_create(&tC, NULL, threadC, NULL);

    // Wait for all threads to finish
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);

    // Destroy semaphores
    sem_destroy(&semA);
    sem_destroy(&semB);
    sem_destroy(&semC);

    return 0;
}
