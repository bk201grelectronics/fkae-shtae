# Byte bitmap

## Table of contents

- [What it is](#what-it-is)
- [How to use it](#how-to-use-it)
- [How to clean it up](#how-to-clean-it-up)
- [API summary](#api-summary)
- [Behavior and safety](#behavior-and-safety)

## What it is

`ByteBitMap` stores Boolean flags as individual bits in caller-provided byte
storage. It is useful when a byte per flag would waste memory.

## How to use it

Allocate enough bytes for the requested bit count and create the bitmap:

```c
#include <limits.h>

#include <libfkae-shtae/bitmap/byteBitMap.h>

enum { BIT_COUNT = 20 };
unsigned char storage[(BIT_COUNT + CHAR_BIT - 1) / CHAR_BIT];
ByteBitMap bitmap = byteBitMapCreate(storage, BIT_COUNT);

bitmap.vtable->set(&bitmap, 13);
bool value = bitmap.vtable->isSet(&bitmap, 13);
bitmap.vtable->clear(&bitmap, 13);
```

The constructor clears the supplied bytes. Bit indexes start at zero and must
be smaller than `bitCount`.

## How to clean it up

```c
bitmap.vtable->destroy(&bitmap);
```

The public constructor borrows storage, so `destroy` invalidates the bitmap but
does not free the caller's byte array. `reset` clears every bit while keeping
the bitmap usable.

## API summary

| Operation | Purpose |
| --- | --- |
| `byteBitMapCreate` | Create a bitmap over caller-owned storage. |
| `set` | Set one bit to one. |
| `clear` | Clear one bit to zero. |
| `isSet` | Report whether one bit is set. |
| `reset` | Clear all bits. |
| `destroy` | Invalidate the bitmap. |

## Behavior and safety

- `set`, `clear`, and `isSet` reject indexes outside the bitmap.
- The bitmap uses `CHAR_BIT`, so it does not assume eight-bit bytes.
- The implementation is not thread-safe.
- The public API currently exposes only caller-owned storage creation.
