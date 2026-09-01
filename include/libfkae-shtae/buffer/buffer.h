#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct BufferVTable BufferVTable;

struct BufferVTable {
  bool (*push)(void *this, const void *item);
  bool (*pop)(void *this, void *item);
  bool (*isEmpty)(void *this);
  bool (*isFull)(void *this);
  size_t (*size)(void *this);
  bool (*reset)(void *this);
};
