#include <assert.h>
#include <stdio.h>

#include "libfkae-shtae/buffer/ring_buffer.h"

int main(void) {

  int storage[4];

  RingBuffer buffer = ringBufferCreate(storage, 4, sizeof(int));

  int input = 42;
  bool pushed = buffer.vtable->push(&buffer, &input);
  assert(pushed == true);
  assert(buffer.vtable->size(&buffer) == 1);

  int output = 0;
  bool popped = buffer.vtable->pop(&buffer, &output);
  assert(popped == true);
  assert(output == 42);
  assert(buffer.vtable->size(&buffer) == 0);
  assert(buffer.vtable->isEmpty(&buffer));

  RingBuffer dBuffer = ringBufferCreateDynamic(4, sizeof(int));
  assert(dBuffer.data != NULL);
  assert(dBuffer.vtable->destroy(&dBuffer) == true);

  printf("RingBuffer read/write test passed\n");
  return 0;
}
