#pragma once

#include "libfkae-shtae/container/container.h"
#include <stddef.h>

typedef struct Array Array;

struct Array {
  const struct ContainerVTable *vtable;
  void *data;
  size_t capacity;
  size_t count;
  size_t elementSize;
  bool ownsTheData;
};

Array arrayCreate(void *storage, size_t capacity, size_t elementSize);
Array arrayCreateDynamic(size_t capacity, size_t elementSize);
