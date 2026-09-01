#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct RingBuffer RingBuffer;
typedef struct RingBufferVTable RingBufferVTable;

struct RingBufferVTable {
  bool (*push)(RingBuffer *this, const void *item);
  bool (*pop)(RingBuffer *this, void *item);
  bool (*isEmpty)(RingBuffer *this);
  bool (*isFull)(RingBuffer *this);
  size_t (*size)(RingBuffer *this);
  void (*reset)(RingBuffer *this);
};

struct RingBuffer {
  const struct RingBufferVTable *vtable;
  void *data;
  size_t capacity;
  size_t elementSize;
  size_t readIndex;
  size_t writeIndex;
  size_t count;
};

RingBuffer ringBufferCreate(void *storage, size_t capacity, size_t elementSize);
