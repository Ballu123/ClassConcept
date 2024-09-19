/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: Thread_PrintABC_seq_CondVar.c
 * Platform: Linux
 * Threads printing the A, B and C in sequence
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t lock;
pthread_cond_t cond_A, cond_B, cond_C;

int turn = 0;  // To keep track of whose turn it is (0 for A, 1 for B, 2 for C)

void* threadA(void* arg) {
    pthread_mutex_lock(&lock);

    while (turn != 0) {
        pthread_cond_wait(&cond_A, &lock);  // Wait for its turn (turn == 0)
    }

    // Thread A prints its part
    printf("Thread A\n");

    turn = 1;  // Set turn to 1 for Thread B
    pthread_cond_signal(&cond_B);  // Signal Thread B

    pthread_mutex_unlock(&lock);
    return NULL;
}

void* threadB(void* arg) {
    pthread_mutex_lock(&lock);

    while (turn != 1) {
        pthread_cond_wait(&cond_B, &lock);  // Wait for its turn (turn == 1)
    }

    // Thread B prints its part
    printf("Thread B\n");

    turn = 2;  // Set turn to 2 for Thread C
    pthread_cond_signal(&cond_C);  // Signal Thread C

    pthread_mutex_unlock(&lock);
    return NULL;
}

void* threadC(void* arg) {
    pthread_mutex_lock(&lock);

    while (turn != 2) {
        pthread_cond_wait(&cond_C, &lock);  // Wait for its turn (turn == 2)
    }

    // Thread C prints its part
    printf("Thread C\n");

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t tA, tB, tC;

    // Initialize the mutex and condition variables
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond_A, NULL);
    pthread_cond_init(&cond_B, NULL);
    pthread_cond_init(&cond_C, NULL);

    // Create threads
    pthread_create(&tA, NULL, threadA, NULL);
    pthread_create(&tB, NULL, threadB, NULL);
    pthread_create(&tC, NULL, threadC, NULL);

    // Signal Thread A to start
    pthread_cond_signal(&cond_A);

    // Wait for all threads to complete
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond_A);
    pthread_cond_destroy(&cond_B);
    pthread_cond_destroy(&cond_C);

    return 0;
}
