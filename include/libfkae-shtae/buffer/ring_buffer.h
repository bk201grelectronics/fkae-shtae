#pragma once

#include "libfkae-shtae/buffer/buffer.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct RingBuffer RingBuffer;

struct RingBuffer {
  const struct BufferVTable *vtable;
  void *data;
  size_t capacity;
  size_t elementSize;
  size_t readIndex;
  size_t writeIndex;
  size_t count;
  bool ownsTheData;
};

RingBuffer ringBufferCreate(void *storage, size_t capacity, size_t elementSize);
RingBuffer ringBufferCreateDynamic(size_t capacity, size_t elementSize);
