#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct MapVTable MapVTable;

struct MapVTable {
  bool (*put)(void *this, const void *key, const void *value);
  bool (*get)(void *this, const void *key, void *value);
  bool (*remove)(void *this, const void *key);
  bool (*contains)(void *this, const void *key);
  bool (*isEmpty)(void *this);
  bool (*isFull)(void *this);
  bool (*reset)(void *this);
  bool (*destroy)(void *this);
  size_t (*size)(void *this);
};
