#pragma once

#include "libfkae-shtae/map/map_vtable.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct HashMap HashMap;
typedef size_t (*HashFunction)(const void *key);
typedef bool (*KeyEqualsFunction)(const void *left, const void *right);

typedef enum {
  HASH_MAP_ENTRY_EMPTY,
  HASH_MAP_ENTRY_OCCUPIED,
  HASH_MAP_ENTRY_DELETED
} HashMapEntryState;

struct HashMap {
  const MapVTable *vtable;
  HashMapEntryState *states;

  void *storage;
  size_t capacity;
  size_t count;

  size_t keySize;
  size_t valueSize;

  HashFunction hash;
  KeyEqualsFunction equals;
};

HashMap hashMapCreate(void *storage, HashMapEntryState *states, size_t capacity,
                      size_t keySize, size_t valueSize, HashFunction hash,
                      KeyEqualsFunction equals);
