#pragma once

#include <stddef.h>

#include "libfkae-shtae/bitmap/bitmap.h"

typedef struct ByteBitMap ByteBitMap;

struct ByteBitMap {
  const struct BitmapVTable *vtable;
  const struct BitmapVDTable *vDtable;
  unsigned char *data;
  size_t bitCount;
  size_t byteCount;
  bool ownsTheStorage;
};

ByteBitMap byteBitMapCreate(unsigned char *storage, size_t bitCount);
