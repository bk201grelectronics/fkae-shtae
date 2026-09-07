#include "libfkae-shtae/container/array.h"
#include "libfkae-shtae/container/container.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool arraySet(void *array, size_t index, const void *item) {

  Array *this = (Array *)array;
  if (this == NULL || this->data == NULL || item == NULL ||
      index >= this->capacity) {
    goto catch;
  }

  unsigned char *destination =
      (unsigned char *)this->data + (index * this->elementSize);
  memcpy(destination, item, this->elementSize);

  if (index >= this->count) {
    this->count = index + 1;
  }

  return true;
catch:
  return false;
}

static bool arrayGet(void *array, size_t index, void *item) {

  Array *this = (Array *)array;
  if (this == NULL || this->data == NULL || item == NULL ||
      index >= this->count) {
    goto catch;
  }
  unsigned char *source =
      (unsigned char *)this->data + (index * this->elementSize);
  memcpy(item, source, this->elementSize);
  return true;
catch:
  return false;
}

static size_t arraySize(void *array) {
  Array *this = (Array *)array;

  if (this == NULL) {
    goto catch;
  }
  return this->count;
catch:
  return 0;
}

static size_t arrayCapacity(void *array) {
  Array *this = (Array *)array;

  if (this == NULL) {
    goto catch;
  }
  return this->capacity;
catch:
  return 0;
}

static void arrayReset(void *array) {
  Array *this = (Array *)array;
  if (this == NULL || this->data == NULL) {
    goto catch;
  }
  memset(this->data, 0, this->capacity * this->elementSize);
  this->count = 0;
  return;
catch:
  return;
}

static bool destroy(void *array) {
  Array *this = (Array *)array;
  if (this == NULL) {
    goto catch;
  }
  if (this->ownsTheData && this->data != NULL) {
    free(this->data);
  }
  this->data = NULL;
  this->count = 0;
  this->capacity = 0;
  this->elementSize = 0;
  this->ownsTheData = false;
  return true;
catch:
  return false;
}

static const ContainerVTable arrayVTable = {
    .set = arraySet,
    .get = arrayGet,
    .capacity = arrayCapacity,
    .size = arraySize,
    .reset = arrayReset,
    .destroy = destroy,
};

Array arrayCreate(void *storage, size_t capacity, size_t elementSize) {
  Array array = {
      .data = storage,
      .vtable = &arrayVTable,
      .capacity = capacity,
      .elementSize = elementSize,
      .count = 0,
      .ownsTheData = false,
  };
  return array;
}

Array arrayCreateDynamic(size_t capacity, size_t elementSize) {
  void *storage = calloc(capacity, elementSize);
  Array array = {
      .data = storage,
      .vtable = &arrayVTable,
      .capacity = capacity,
      .elementSize = elementSize,
      .count = 0,
      .ownsTheData = true,
  };
  return array;
}
