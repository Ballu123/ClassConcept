/*
 * Name:Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: AlignedMalloc.cpp
 * Win10
 * Microsoft Visual Studio
 */
=====
                        returned pointer
                              |
                              v
0x1000       0x1004       0x1010
   |             |            |
   v             v            v
+------+---------+------------+----------------------+
|      |         |            |                      |
|      | metadata| unused     |    USER MEMORY       |
|      | 16      |            |       100 bytes      |
+------+---------+------------+----------------------+
       <-------->
        12 + 4
Actually, the metadata sits immediately before the returned pointer:
                 returned pointer
                       |
                       v
              +--------+----------------------+
              | offset |      user data       |
              +--------+----------------------+
              ^
              |
        aligned address - 4
 
The stored value is:
offset + 4
So free() knows how far backward to go.

=====
malloc()
   |
   v
+--------+--------+-------------------+
| extra  | offset | aligned user data |
+--------+--------+-------------------+
                   ^
                   |
                 return

free():
return ptr
    ↓
read offset
    ↓
recover malloc pointer
    ↓
free(original)
=====
Remember: malloc → move → remember → return and read → move back → free.
====
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void *aligned_malloc(size_t size, size_t alignment)
{
    uintptr_t raw;
    uintptr_t aligned;
    size_t offset;

    /* Allocate extra space for alignment + metadata */
    raw = (uintptr_t)malloc(size + alignment + sizeof(size_t));

    if (raw == 0)
        return NULL;

    /* Move past space reserved for metadata */
    aligned = raw + sizeof(size_t);

    /* Calculate how much to move for alignment */
    offset = alignment - (aligned % alignment);

    /* Move to aligned address */
    aligned += offset;

    /* Store distance back to original malloc pointer */
    *((size_t *)(aligned - sizeof(size_t))) =
        offset + sizeof(size_t);

    return (void *)aligned;
}

void aligned_free(void *ptr)
{
    size_t offset;

    if (ptr == NULL)
        return;

    /* Read stored offset */
    offset = *((size_t *)((uintptr_t)ptr - sizeof(size_t)));

    /* Recover original malloc pointer */
    void *raw = (void *)((uintptr_t)ptr - offset);

    free(raw);
}

int main()
{
    void *ptr = aligned_malloc(100, 64);

    if (ptr == NULL)
        return 1;

    printf("Address = %p\n", ptr);
    printf("Address %% 64 = %lu\n",
           (unsigned long)((uintptr_t)ptr % 64));

    aligned_free(ptr);

    return 0;
}
