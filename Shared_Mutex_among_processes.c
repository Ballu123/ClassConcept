/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: Shared_Mutex_among_processes.c
 * Platform: Linux
 * To transfer a mutex among multiple processes, you need to use an inter-process mutex. 
 * Unlike a regular mutex, which is only valid within a single process, an inter-process mutex allows synchronization between different processes. 
 * This can be achieved using platform-specific mechanisms. On POSIX systems (e.g., Linux), you typically use POSIX shared mutexes or System V semaphores to accomplish this.
 */

#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t *mutex;

void init_mutex_in_shared_memory() {
    // Create a file-backed shared memory object
    int shm_fd = shm_open("/myshm", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Resize the shared memory object
    ftruncate(shm_fd, sizeof(pthread_mutex_t));

    // Map shared memory into process address space
    mutex = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (mutex == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Initialize the mutex with the shared attribute
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &attr);
}

int main() {
    // Initialize the mutex in shared memory
    init_mutex_in_shared_memory();

    // Lock the mutex
    printf("Process %d locking mutex\n", getpid());
    pthread_mutex_lock(mutex);

    // Critical section
    printf("Process %d in critical section\n", getpid());
    sleep(2);

    // Unlock the mutex
    printf("Process %d unlocking mutex\n", getpid());
    pthread_mutex_unlock(mutex);

    // Cleanup: Unmap and unlink shared memory
    munmap(mutex, sizeof(pthread_mutex_t));
    shm_unlink("/myshm");

    return 0;
}
