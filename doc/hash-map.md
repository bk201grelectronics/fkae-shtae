# Hash map

## Table of contents

- [What it is](#what-it-is)
- [Build status](#build-status)
- [How to use it](#how-to-use-it)
- [How to clean it up](#how-to-clean-it-up)
- [API summary](#api-summary)
- [Behavior and safety](#behavior-and-safety)

## What it is

`HashMap` is a fixed-capacity generic hash map that uses linear probing. The
caller provides entry storage, slot states, a hash function, and a key equality
function.

## Build status

The hash map is currently source-only. Its header and implementation are in the
repository, but `src/map/hash_map.c` is not included in the Makefile or static
archive. Integrate and test it before treating it as an installed feature.

## How to use it

The storage needs `capacity * (keySize + valueSize)` bytes. The state array
needs one `HashMapEntryState` for each entry.

```c
#include <string.h>

#include <libfkae-shtae/map/hash_map.h>

static size_t hashInt(const void *key) {
  return (size_t)*(const int *)key;
}

static bool intEquals(const void *left, const void *right) {
  return memcmp(left, right, sizeof(int)) == 0;
}

enum { CAPACITY = 8 };
unsigned char storage[CAPACITY * (sizeof(int) + sizeof(float))];
HashMapEntryState states[CAPACITY];

HashMap map = hashMapCreate(
    storage,
    states,
    CAPACITY,
    sizeof(int),
    sizeof(float),
    hashInt,
    intEquals);

int key = 3;
float input = 1.5f;
float output = 0.0f;

map.vtable->put(&map, &key, &input);
map.vtable->get(&map, &key, &output);
```

## How to clean it up

```c
map.vtable->destroy(&map);
```

The map borrows both arrays. `destroy` invalidates the map but does not free the
storage or states. `reset` clears storage and marks every slot empty while
keeping the map usable.

## API summary

| Operation | Purpose |
| --- | --- |
| `hashMapCreate` | Create a map over caller-owned arrays. |
| `put` | Insert a key/value pair or replace an existing value. |
| `get` | Copy a value for a key into caller-provided output. |
| `remove` | Mark a key's slot as deleted. |
| `contains` | Report whether a key exists. |
| `isEmpty` | Report whether the map contains no entries. |
| `isFull` | Report whether all slots are occupied. |
| `size` | Return the number of occupied entries. |
| `reset` | Clear all entries without releasing caller storage. |
| `destroy` | Invalidate the map without freeing caller storage. |

## Behavior and safety

- Keys and values are copied as fixed-size byte sequences.
- Capacity is fixed; the map does not resize.
- Removed slots can be reused by later insertions.
- Hash and equality callbacks must remain valid for the map's lifetime.
- The implementation is not thread-safe.
