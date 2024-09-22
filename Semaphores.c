/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: Semaphores.c
 * Platform: Linux
 * To rotate an array to the left by k positions using a thread and semaphore,
 */


#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// Define the maximum array size and the semaphore
#define MAX_SIZE 100
sem_t mutex;

// Define the struct to hold the array and its size
struct Array {
    int n;
    int arr[MAX_SIZE];
};

// Function to print the array (for debugging purposes)
void print(void *arg) {
    struct Array *ar = (struct Array*) arg;
    for (int i = 0; i < ar->n; i++) {
        printf("%d ", ar->arr[i]);
    }
    printf("\n");
}

// Function to rotate the array by one position to the left
void *rotate_by_one(void *arg) {
    struct Array *ar = (struct Array*) arg;

    // Wait for the semaphore
    sem_wait(&mutex);

    // Rotate the array by one position
    int first = ar->arr[0];
    for (int i = 0; i < ar->n - 1; i++) {
        ar->arr[i] = ar->arr[i + 1];
    }
    ar->arr[ar->n - 1] = first;

    // Print the array after each rotation
    print(ar);

    // Signal the semaphore
    sem_post(&mutex);

    return NULL;
}

int main() {
    sem_init(&mutex, 0, 1); // Initialize the semaphore with a value of 1

    int n, k;
    scanf("%d %d", &n, &k); // Input the size of the array and the number of rotations

    struct Array obj;
    obj.n = n;

    // Input the array elements
    for (int i = 0; i < n; i++) {
        scanf("%d", &obj.arr[i]);
    }

    // Create threads for each rotation
    pthread_t t[k];
    for (int i = 0; i < k; i++) {
        pthread_create(&t[i], NULL, rotate_by_one, (void*)&obj);
    }

    // Join the threads to ensure all rotations are complete
    for (int i = 0; i < k; i++) {
        pthread_join(t[i], NULL);
    }

    // Destroy the semaphore
    sem_destroy(&mutex);
    
    return 0;
}
