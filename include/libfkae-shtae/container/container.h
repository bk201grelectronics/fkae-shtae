
#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct ContainerVTable ContainerVTable;

struct ContainerVTable {
  bool (*set)(void *this, size_t index, const void *item);
  bool (*get)(void *this, size_t index, void *item);
  size_t (*size)(void *this);
  size_t (*capacity)(void *this);
  void (*reset)(void *this);
};
