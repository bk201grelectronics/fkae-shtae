#include <stdlib.h>
#include <string.h>

#include "libfkae-shtae/buffer/buffer.h"
#include "libfkae-shtae/buffer/constants.h"
#include "libfkae-shtae/buffer/ring_buffer.h"

static bool ringBufferIsEmpty(void *buffer) {
  RingBuffer *this = (RingBuffer *)buffer;
  return this->count == 0;
}

static bool ringBufferIsFull(void *buffer) {
  RingBuffer *this = (RingBuffer *)buffer;
  return this->count == this->capacity;
}

static size_t ringBufferSize(void *buffer) {
  RingBuffer *this = (RingBuffer *)buffer;
  return this->count;
}

static bool ringBufferPush(void *buffer, const void *item) {
  RingBuffer *this = (RingBuffer *)buffer;
  if (this == NULL || item == NULL) {
    goto catch;
  }

  if (this->vtable->isFull(this)) {
    goto catch;
  }

  unsigned char *destination =
      (unsigned char *)this->data + (this->writeIndex * this->elementSize);

  memcpy(destination, item, this->elementSize);
  this->writeIndex = (this->writeIndex + 1) % this->capacity;

  this->count++;
  return true;

  catch : return false;
}

static bool ringBufferPop(void *buffer, void *item) {
  RingBuffer *this = (RingBuffer *)buffer;
  if (this == NULL || item == NULL) {
    goto catch;
  }
  if (this->vtable->isEmpty(this)) {
    goto catch;
  }
  unsigned char *source =
      (unsigned char *)this->data + (this->readIndex * this->elementSize);
  memcpy(item, source, this->elementSize);
  this->readIndex = (this->readIndex + 1) % this->capacity;
  this->count--;
  return true;
  catch : return false;
}

static bool ringBufferReset(void *buffer) {
  RingBuffer *this = (RingBuffer *)buffer;
  this->readIndex = 0;
  this->writeIndex = 0;
  this->count = 0;
  return true;
}

static bool ringBufferDestroy(void *buffer) {
  RingBuffer *this = (RingBuffer *)buffer;
  if (this == NULL) {
    goto catch;
  }
  if (this->ownsTheData && this->data != NULL) {
    free(this->data);
  }

  this->data = NULL;
  this->capacity = 0;
  this->ownsTheData = false;
  this->readIndex = 0;
  this->writeIndex = 0;
  this->elementSize = 0;
  this->count = 0;
  return true;
  catch : return false;
}

static const BufferVTable ringBufferVTable = {
    .push = ringBufferPush,
    .pop = ringBufferPop,
    .isEmpty = ringBufferIsEmpty,
    .isFull = ringBufferIsFull,
    .size = ringBufferSize,
    .reset = ringBufferReset,
    .destroy = ringBufferDestroy,
};

RingBuffer ringBufferCreate(void *storage, size_t capacity,
                            size_t elementSize) {

  RingBuffer buffer = {
      .vtable = &ringBufferVTable,
      .data = storage,
      .capacity = capacity,
      .elementSize = elementSize,
      .readIndex = 0,
      .writeIndex = 0,
      .count = 0,
      .ownsTheData = false,
  };
  return buffer;
}

RingBuffer ringBufferCreateDynamic(size_t capacity, size_t elementSize) {
  void *storage = calloc(capacity, elementSize);
  RingBuffer buffer = {
      .vtable = &ringBufferVTable,
      .data = storage,
      .capacity = capacity,
      .elementSize = elementSize,
      .readIndex = 0,
      .writeIndex = 0,
      .count = 0,
      .ownsTheData = true,
  };
  return buffer;
}
