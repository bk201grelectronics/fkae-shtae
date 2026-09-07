#include <assert.h>
#include <stdio.h>

#include "libfkae-shtae/container/array.h"

int main(void) {
  int storage[5];

  Array array = arrayCreate(storage, 5, sizeof(int));

  /*
   * Initially empty.
   */
  assert(array.vtable->size(&array) == 0);
  assert(array.vtable->capacity(&array) == 5);

  /*
   * Set index 0.
   */
  int value = 10;
  assert(array.vtable->set(&array, 0, &value) == true);
  assert(array.vtable->size(&array) == 1);

  /*
   * Get index 0.
   */
  int result = 0;
  assert(array.vtable->get(&array, 0, &result) == true);
  assert(result == 10);

  /*
   * Set another value.
   */
  value = 25;
  assert(array.vtable->set(&array, 1, &value) == true);
  assert(array.vtable->size(&array) == 2);

  result = 0;
  assert(array.vtable->get(&array, 1, &result) == true);
  assert(result == 25);

  /*
   * Out of bounds set should fail.
   */
  value = 99;
  assert(array.vtable->set(&array, 5, &value) == false);

  /*
   * Reset array.
   */
  array.vtable->reset(&array);
  assert(array.vtable->size(&array) == 0);

  array.vtable->destroy(&array);

  printf("Array set/get/reset test passed\n");

  return 0;
}
