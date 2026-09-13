# Array

## Table of contents

- [What it is](#what-it-is)
- [How to use it](#how-to-use-it)
- [How to clean it up](#how-to-clean-it-up)
- [API summary](#api-summary)
- [Behavior and safety](#behavior-and-safety)

## What it is

`Array` is a fixed-capacity generic array. It copies fixed-size values into
indexed slots and tracks a logical size separately from its capacity.

The normal constructor borrows storage from the caller. The dynamic constructor
allocates zero-initialized storage with `calloc`.

## How to use it

```c
#include <libfkae-shtae/container/array.h>

int storage[5] = {0};
Array array = arrayCreate(storage, 5, sizeof(int));

int input = 25;
int output = 0;

bool stored = array.vtable->set(&array, 1, &input);
bool loaded = array.vtable->get(&array, 1, &output);
```

For library-owned storage:

```c
Array array = arrayCreateDynamic(5, sizeof(int));

if (array.data == NULL) {
  /* Allocation failed. */
}
```

Setting index `n` grows the logical size to `n + 1` when needed. Getting an
index at or beyond the logical size fails, even when it is below capacity.

## How to clean it up

```c
array.vtable->destroy(&array);
```

`destroy` frees storage created by `arrayCreateDynamic`. It does not free
storage passed to `arrayCreate`.

`reset` zeroes every storage byte and sets the logical size to zero while
keeping the array usable.

## API summary

| Operation | Purpose |
| --- | --- |
| `arrayCreate` | Create an array over caller-owned storage. |
| `arrayCreateDynamic` | Create an array with allocated storage. |
| `set` | Copy a value into an index below capacity. |
| `get` | Copy a value from an index below logical size. |
| `size` | Return the logical size. |
| `capacity` | Return the maximum number of elements. |
| `reset` | Zero storage and set logical size to zero. |
| `destroy` | Free owned storage and invalidate the object. |

## Behavior and safety

- Values are copied using `elementSize`; type information is not stored.
- The array does not resize itself.
- The implementation is not thread-safe.
- Use a non-null storage pointer, non-zero capacity, and non-zero element size.
