/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: CircularBuffer_ThreadSafe.c
 * Win10
 * Microsoft Visual Studio
 * A circular buffer is a FIFO data structure that treats memory as circular; that is, the read/write indices loop back to 0 after they reach the buffer length. 
 * This is achieved by two pointers to the array, the “head” pointer and the “tail” pointer. 
 * As data is added (write) to the buffer, the head pointer is incremented, and likewise, when the data is being removed (read), the tail pointer is incremented. 
 * The definition of head, tail, their movement direction, and write and read location are all implementation-dependent,
 * but the idea/goal remains the same.
 */
  Producer                   Consumer
     │                           │
  lock mutex                  lock mutex
     │                           │
  is_full?──YES──wait()       is_empty?──YES──wait()
     │                           │
  write data                  read data
     │                           │
  signal(not_empty)           signal(not_full)
     │                           │
  unlock mutex                unlock mutex
//Three things make it thread-safe: a mutex guards all reads/writes, not_empty blocks the consumer until data arrives, 
//and not_full blocks the producer when the buffer is full — so neither thread ever sees corrupt state.

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

/*─────────────────────────────────────────
  Circular Buffer — Simple & Thread-Safe
─────────────────────────────────────────*/

#define BUF_SIZE 8   // must be power of 2

typedef struct {
    uint8_t         buf[BUF_SIZE];
    uint32_t        head;   // write index
    uint32_t        tail;   // read index
    pthread_mutex_t lock;
    pthread_cond_t  not_empty;
    pthread_cond_t  not_full;
} CircBuf;

/*──── Init ────────────────────────────────*/
void cb_init(CircBuf *cb) {
    cb->head = 0;
    cb->tail = 0;
    memset(cb->buf, 0, BUF_SIZE);
    pthread_mutex_init(&cb->lock,      NULL);
    pthread_cond_init(&cb->not_empty,  NULL);
    pthread_cond_init(&cb->not_full,   NULL);
}

/*──── Full / Empty checks ─────────────────*/
static bool is_full(CircBuf *cb) {
    return ((cb->head + 1) % BUF_SIZE) == cb->tail;
}

static bool is_empty(CircBuf *cb) {
    return cb->head == cb->tail;
}

/*──── Push (blocks if full) ───────────────*/
void cb_push(CircBuf *cb, uint8_t data) {
    pthread_mutex_lock(&cb->lock);

    while (is_full(cb))                         // wait for space
        pthread_cond_wait(&cb->not_full, &cb->lock);

    cb->buf[cb->head] = data;
    cb->head = (cb->head + 1) % BUF_SIZE;

    pthread_cond_signal(&cb->not_empty);        // wake consumer
    pthread_mutex_unlock(&cb->lock);
}

/*──── Pop (blocks if empty) ───────────────*/
uint8_t cb_pop(CircBuf *cb) {
    pthread_mutex_lock(&cb->lock);

    while (is_empty(cb))                        // wait for data
        pthread_cond_wait(&cb->not_empty, &cb->lock);

    uint8_t data = cb->buf[cb->tail];
    cb->tail = (cb->tail + 1) % BUF_SIZE;

    pthread_cond_signal(&cb->not_full);         // wake producer
    pthread_mutex_unlock(&cb->lock);

    return data;
}

/*──── Destroy ─────────────────────────────*/
void cb_destroy(CircBuf *cb) {
    pthread_mutex_destroy(&cb->lock);
    pthread_cond_destroy(&cb->not_empty);
    pthread_cond_destroy(&cb->not_full);
}

/*─────────────────────────────────────────
  Test — Producer & Consumer Threads
─────────────────────────────────────────*/
CircBuf shared_buf;

void *producer(void *arg) {
    uint8_t items[] = {1, 2, 3, 4, 5, 6, 7, 8};
    for (int i = 0; i < 8; i++) {
        cb_push(&shared_buf, items[i]);
        printf("[Producer] pushed: %d\n", items[i]);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < 8; i++) {
        uint8_t val = cb_pop(&shared_buf);
        printf("[Consumer] popped: %d\n", val);
    }
    return NULL;
}

int main(void) {
    cb_init(&shared_buf);

    pthread_t prod, cons;
    pthread_create(&cons, NULL, consumer, NULL);  // start consumer first
    pthread_create(&prod, NULL, producer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    cb_destroy(&shared_buf);
    printf("\nDone!\n");
    return 0;
}
