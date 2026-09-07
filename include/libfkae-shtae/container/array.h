#pragma once

#include <stddef.h>

typedef struct Array Array;

struct Array {
  const struct ContainerV *vtable;
  void *data;
}
