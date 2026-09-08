
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
  return true;
  catch : return false;
}
