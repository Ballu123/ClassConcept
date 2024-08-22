/*
 * Name:Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: Validate_1GB_Memory.c
 * Win10
 * Microsoft Visual Studio
 *To validate that 1 GB of memory is correctly copied from a source to a destination
 * where the positions of bytes do not matter (i.e., as long as all bytes are present in the destination, their order does not matter),
 * you can use a checksum that is order-independent. 
 *One simple approach is to calculate the sum of all bytes (i.e., the cumulative sum of all byte values) since the order of addition does not matter.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Constants
#define SIZE_IN_BYTES (1UL * 1024 * 1024 * 1024) // 1 GB

// Function to calculate a simple order-independent checksum (sum of all bytes)
uint64_t calculate_checksum(uint8_t *data, size_t size) {
    uint64_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum += data[i];
    }
    return checksum;
}

// Function to simulate a black box copy operation
void black_box_copy(uint8_t *source, uint8_t *destination, size_t size) {
    // This copy does not preserve the order of bytes, but we assume all bytes are copied
    // Here we'll simply shuffle the bytes randomly as an example
    for (size_t i = 0; i < size; i++) {
        size_t rand_index = rand() % size;
        destination[i] = source[rand_index];
    }
}

int main() {
    // Allocate memory for source and destination
    uint8_t *source = (uint8_t *)malloc(SIZE_IN_BYTES);
    uint8_t *destination = (uint8_t *)malloc(SIZE_IN_BYTES);

    if (source == NULL || destination == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Initialize source memory with random data
    for (size_t i = 0; i < SIZE_IN_BYTES; i++) {
        source[i] = rand() % 256; // Fill with random byte values
    }

    // Step 1: Calculate checksum for source memory
    uint64_t source_checksum = calculate_checksum(source, SIZE_IN_BYTES);
    printf("Source checksum: 0x%016lX\n", source_checksum);

    // Step 2: Perform the black box copy
    black_box_copy(source, destination, SIZE_IN_BYTES);

    // Step 3: Calculate checksum for destination memory
    uint64_t destination_checksum = calculate_checksum(destination, SIZE_IN_BYTES);
    printf("Destination checksum: 0x%016lX\n", destination_checksum);

    // Step 4: Compare checksums
    if (source_checksum == destination_checksum) {
        printf("Data integrity validated successfully. No corruption detected.\n");
    } else {
        printf("Data corruption detected! Checksum mismatch.\n");
    }

    // Free allocated memory
    free(source);
    free(destination);

    return 0;
}
