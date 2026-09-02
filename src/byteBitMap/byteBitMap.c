#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "libfkae-shtae/bitmap/bitmap.h"
#include "libfkae-shtae/bitmap/byteBitMap.h"

static size_t getByteIndex(size_t index) { return index / CHAR_BIT; }
static size_t getBitIndex(size_t index) { return index % CHAR_BIT; }
static unsigned char getMask(size_t bitIndex) {
  return (unsigned char)(1u << bitIndex);
}

static bool byteBitMapSet(void *bitMap, size_t index) {
  ByteBitMap *this = (ByteBitMap *)bitMap;
  if (this == NULL || index >= this->bitCount) {
    goto catch;
  }
  size_t byteIndex = getByteIndex(index);
  size_t bitIndex = getBitIndex(index);
  unsigned char mask = getMask(bitIndex);
  this->data[byteIndex] |= mask;

  return true;
  catch : return false;
}

static bool byteBitMapClear(void *bitMap, size_t index) {
  ByteBitMap *this = (ByteBitMap *)bitMap;
  if (this == NULL || index >= this->bitCount) {
    goto catch;
  }
  size_t byteIndex = getByteIndex(index);
  size_t bitIndex = getBitIndex(index);
  unsigned char mask = getMask(bitIndex);

  this->data[byteIndex] = this->data[byteIndex] & ~mask;
  return true;
  catch : return false;
}

static bool byteBitMapIsSet(void *bitMap, size_t index) {
  ByteBitMap *this = (ByteBitMap *)bitMap;
  if (this == NULL || index >= this->bitCount) {
    goto catch;
  }
  size_t byteIndex = getByteIndex(index);
  size_t bitIndex = getBitIndex(index);
  unsigned char mask = getMask(bitIndex);

  return (this->data[byteIndex] & mask) != 0;
  catch : return false;
}

static bool byteBitMapReset(void *bitMap) {
  ByteBitMap *this = (ByteBitMap *)bitMap;
  if (this == NULL) {
    goto catch;
  }

  memset(this->data, 0, this->byteCount);

  return true;

  catch : return false;
}

static bool byteBitMapDestroy(void *bitMap) {
  ByteBitMap *this = (ByteBitMap *)bitMap;

  if (this == NULL) {
    goto catch;
  }

  if (this->ownsTheStorage && this->data != NULL) {
    free(this->data);
  }

  this->data = NULL;
  this->bitCount = 0;
  this->byteCount = 0;
  this->ownsTheStorage = false;

  return true;
  catch : return false;
}

static const BitmapVTable byteBitMapVTable = {
    .set = byteBitMapSet,
    .clear = byteBitMapClear,
    .isSet = byteBitMapIsSet,
    .reset = byteBitMapReset,
    .destroy = byteBitMapDestroy,
};

ByteBitMap byteBitMapCreate(unsigned char *storage, size_t bitCount) {
  size_t byteCount = (bitCount + CHAR_BIT - 1) / CHAR_BIT;

  ByteBitMap bitMap = {
      .vtable = &byteBitMapVTable,
      .data = storage,
      .byteCount = byteCount,
      .bitCount = bitCount,
      .ownsTheStorage = false,
  };
  if (storage != NULL) {
    memset(storage, 0, byteCount);
  }
  return bitMap;
}

ByteBitMap byteBitMapCreateDynamic(size_t bitCount) {
  size_t byteCount = (bitCount + CHAR_BIT - 1) / CHAR_BIT;

  unsigned char *storage = calloc(byteCount, sizeof(unsigned char));

  ByteBitMap bitMap = {
      .vtable = &byteBitMapVTable,
      .data = storage,
      .byteCount = byteCount,
      .bitCount = bitCount,
      .ownsTheStorage = true,
  };

  return bitMap;
}
