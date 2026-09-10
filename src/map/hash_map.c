#include "libfkae-shtae/map/hash_map.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static bool hashMapPut(void *map, const void *key, const void *value) {
  HashMap *this = (HashMap *)map;

  if (this == NULL || this->storage == NULL || this->states == NULL ||
      this->hash == NULL || this->equals == NULL || key == NULL ||
      value == NULL || this->capacity == 0) {
    goto catch;
  }

  size_t startIndex = this->hash(key) % this->capacity;
  size_t deletedIndex = this->capacity;

  for (size_t offset = 0; offset < this->capacity; offset++) {
    size_t index = (startIndex + offset) % this->capacity;

    unsigned char *entry = (unsigned char *)this->storage +
                           (index * (this->keySize + this->valueSize));

    void *storedKey = entry;
    void *storedValue = entry + this->keySize;

    if (this->states[index] == HASH_MAP_ENTRY_OCCUPIED) {

      if (this->equals(storedKey, key)) {
        memcpy(storedValue, value, this->valueSize);
        return true;
      }

      continue;
    }

    if (this->states[index] == HASH_MAP_ENTRY_DELETED) {

      if (deletedIndex == this->capacity) {
        deletedIndex = index;
      }

      continue;
    }

    /*
     * EMPTY slot found.
     */
    if (this->states[index] == HASH_MAP_ENTRY_EMPTY) {

      /*
       * Prefer an earlier deleted slot if we found one.
       */
      if (deletedIndex != this->capacity) {
        index = deletedIndex;

        entry = (unsigned char *)this->storage +
                (index * (this->keySize + this->valueSize));

        storedKey = entry;
        storedValue = entry + this->keySize;
      }

      memcpy(storedKey, key, this->keySize);
      memcpy(storedValue, value, this->valueSize);

      this->states[index] = HASH_MAP_ENTRY_OCCUPIED;
      this->count++;

      return true;
    }
  }

  /*
   * We may have searched the entire map without finding an EMPTY slot,
   * but there may still be a DELETED slot that can be reused.
   */
  if (deletedIndex != this->capacity) {

    unsigned char *entry = (unsigned char *)this->storage +
                           (deletedIndex * (this->keySize + this->valueSize));

    void *storedKey = entry;
    void *storedValue = entry + this->keySize;

    memcpy(storedKey, key, this->keySize);
    memcpy(storedValue, value, this->valueSize);

    this->states[deletedIndex] = HASH_MAP_ENTRY_OCCUPIED;
    this->count++;

    return true;
  }

catch:
  return false;
}

static bool hashMapGet(void *map, const void *key, void *value) {
  HashMap *this = (HashMap *)map;

  if (this == NULL || this->storage == NULL || this->states == NULL ||
      this->hash == NULL || this->equals == NULL || key == NULL ||
      value == NULL || this->capacity == 0) {
    goto catch;
  }

  size_t startIndex = this->hash(key) % this->capacity;

  for (size_t offset = 0; offset < this->capacity; offset++) {
    size_t index = (startIndex + offset) % this->capacity;

    if (this->states[index] == HASH_MAP_ENTRY_EMPTY) {
      goto catch;
    }

    if (this->states[index] == HASH_MAP_ENTRY_DELETED) {
      continue;
    }

    unsigned char *entry = (unsigned char *)this->storage +
                           (index * (this->keySize + this->valueSize));

    void *storedKey = entry;
    void *storedValue = entry + this->keySize;

    if (this->equals(storedKey, key)) {
      memcpy(value, storedValue, this->valueSize);
      return true;
    }
  }

catch:
  return false;
}

static bool hashMapRemove(void *map, const void *key) {
  HashMap *this = (HashMap *)map;

  if (this == NULL || this->storage == NULL || this->states == NULL ||
      this->hash == NULL || this->equals == NULL || key == NULL ||
      this->capacity == 0) {
    goto catch;
  }

  size_t startIndex = this->hash(key) % this->capacity;

  for (size_t offset = 0; offset < this->capacity; offset++) {
    size_t index = (startIndex + offset) % this->capacity;

    if (this->states[index] == HASH_MAP_ENTRY_EMPTY) {
      goto catch;
    }

    if (this->states[index] == HASH_MAP_ENTRY_DELETED) {
      continue;
    }

    unsigned char *entry = (unsigned char *)this->storage +
                           (index * (this->keySize + this->valueSize));

    void *storedKey = entry;

    if (this->equals(storedKey, key)) {
      this->states[index] = HASH_MAP_ENTRY_DELETED;
      this->count--;

      return true;
    }
  }

catch:
  return false;
}

static bool hashMapContains(void *map, const void *key) {
  HashMap *this = (HashMap *)map;

  if (this == NULL || this->storage == NULL || this->states == NULL ||
      this->hash == NULL || this->equals == NULL || key == NULL ||
      this->capacity == 0) {
    goto catch;
  }

  size_t startIndex = this->hash(key) % this->capacity;

  for (size_t offset = 0; offset < this->capacity; offset++) {
    size_t index = (startIndex + offset) % this->capacity;

    if (this->states[index] == HASH_MAP_ENTRY_EMPTY) {
      goto catch;
    }

    if (this->states[index] == HASH_MAP_ENTRY_DELETED) {
      continue;
    }

    unsigned char *entry = (unsigned char *)this->storage +
                           (index * (this->keySize + this->valueSize));

    void *storedKey = entry;

    if (this->equals(storedKey, key)) {
      return true;
    }
  }

catch:
  return false;
}

static bool hashMapIsEmpty(void *map) {
  HashMap *this = (HashMap *)map;

  if (this == NULL) {
    goto catch;
  }

  return this->count == 0;

catch:
  return false;
}

static bool hashMapIsFull(void *map) {
  HashMap *this = (HashMap *)map;

  if (this == NULL) {
    goto catch;
  }

  return this->count == this->capacity;

catch:
  return false;
}

static bool hashMapReset(void *map) {
  HashMap *this = (HashMap *)map;

  if (this == NULL || this->storage == NULL || this->states == NULL) {
    goto catch;
  }

  memset(this->storage, 0, this->capacity * (this->keySize + this->valueSize));

  for (size_t index = 0; index < this->capacity; index++) {
    this->states[index] = HASH_MAP_ENTRY_EMPTY;
  }

  this->count = 0;

  return true;

catch:
  return false;
}

static bool hashMapDestroy(void *map) {
  HashMap *this = (HashMap *)map;

  if (this == NULL) {
    goto catch;
  }

  this->storage = NULL;
  this->states = NULL;

  this->capacity = 0;
  this->count = 0;

  this->keySize = 0;
  this->valueSize = 0;

  this->hash = NULL;
  this->equals = NULL;

  return true;

catch:
  return false;
}

static size_t hashMapSize(void *map) {
  HashMap *this = (HashMap *)map;

  if (this == NULL) {
    goto catch;
  }

  return this->count;

catch:
  return 0;
}

static const MapVTable hashMapVTable = {
    .put = hashMapPut,
    .get = hashMapGet,
    .remove = hashMapRemove,
    .contains = hashMapContains,
    .isEmpty = hashMapIsEmpty,
    .isFull = hashMapIsFull,
    .reset = hashMapReset,
    .destroy = hashMapDestroy,
    .size = hashMapSize,
};

HashMap hashMapCreate(void *storage, HashMapEntryState *states, size_t capacity,
                      size_t keySize, size_t valueSize, HashFunction hash,
                      KeyEqualsFunction equals) {

  HashMap map = {
      .vtable = &hashMapVTable,
      .states = states,
      .storage = storage,
      .capacity = capacity,
      .count = 0,
      .keySize = keySize,
      .valueSize = valueSize,
      .hash = hash,
      .equals = equals,
  };

  if (storage != NULL && states != NULL) {
    memset(storage, 0, capacity * (keySize + valueSize));

    for (size_t index = 0; index < capacity; index++) {
      states[index] = HASH_MAP_ENTRY_EMPTY;
    }
  }

  return map;
}
