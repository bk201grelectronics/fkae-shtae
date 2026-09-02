#pragma once

#include <stddef.h>

#include <libfkae-shtae/buffer/buffer.h>

typedef struct Array Array;

struct Array {
  const BufferVTable *vtable;
}
