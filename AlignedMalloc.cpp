/*
 * Name:Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: AlignedMalloc.cpp
 * Win10
 * Microsoft Visual Studio
 */
 
#include <iostream>
#include <stdio.h>
#include <malloc.h>

void* aligned_malloc( size_t required_bytes ,size_t alignment ) {
  void* p1 ; //alligned initial block
  void* p2 ;// alligned block inside initial block
 
  // if alignment is 16, we need to allocate an extra 15 required_bytes
  // 15bytes + 100bytes right after that sequences, 
  //Now,memory address divisble by 16 with space for 100 bytes
  int offset = alignment -1 + sizeof(void*);
  if ((p1 = (void*)malloc(required_bytes + offset)) == NULL) {
    return NULL;
  }
 
  // if alignment is 16, then one of the first 16 memory address
  // in the block at p must be divisible by 16. 
  // with (p+ 15) & 11...10000 we advance as need to this address
  // Adding the last four bits of p+ 15 with 0000 guarantees that this new
  //value will be divisible by 16.
  p2 = (void*)(((size_t)(p1) + offset) & ~(alignment - 1)); 
  ((void**)p2)[-1] = p1;
  return p2;
}

void aligned_free(void *p2) {
  void* p1 = ((void**)p2)[-1];
  free(p1);
}

int main (int argc, char *argv[])
{
    int *p = (int*)aligned_malloc(100, 16);
    printf (": %p\n", p);
    aligned_free (p);
    return 0;
}
