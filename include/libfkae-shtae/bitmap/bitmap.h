#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct BitmapVTable BitmapVTable;
typedef struct BitmapVDTable BitmapVDTable;

struct BitmapVTable {
  bool (*set)(void *this, size_t index);
  bool (*clear)(void *this, size_t index);
  bool (*isSet)(void *this, size_t index);
  bool (*reset)(void *this);
  bool (*destroy)(void *this);
};

