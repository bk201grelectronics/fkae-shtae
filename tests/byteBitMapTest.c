#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "libfkae-shtae/bitmap/byteBitMap.h"

int main(void) {
  unsigned char storage[(20 + CHAR_BIT - 1) / CHAR_BIT];

  ByteBitMap bitMap = byteBitMapCreate(storage, 20);

  /*
   * Initially empty.
   */
  assert(bitMap.vtable->isSet(&bitMap, 13) == false);

  /*
   * Set bit 13.
   */
  assert(bitMap.vtable->set(&bitMap, 13) == true);
  assert(bitMap.vtable->isSet(&bitMap, 13) == true);

  /*
   * Clear bit 13.
   */
  assert(bitMap.vtable->clear(&bitMap, 13) == true);
  assert(bitMap.vtable->isSet(&bitMap, 13) == false);

  printf("ByteBitMap set/get/clear test passed\n");

  return 0;
}
