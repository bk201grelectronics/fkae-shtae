# Ring buffer

## Table of contents

- [What it is](#what-it-is)
- [How to use it](#how-to-use-it)
- [How to clean it up](#how-to-clean-it-up)
- [API summary](#api-summary)
- [Behavior and safety](#behavior-and-safety)

## What it is

`RingBuffer` is a fixed-capacity, first-in-first-out container for values of any
fixed-size C type. It tracks read and write indexes and wraps them at the end of
the supplied storage.

The normal constructor borrows storage from the caller. The dynamic constructor
allocates its own storage with `calloc`.

## How to use it

Include the public header:

```c
#include <libfkae-shtae/buffer/ring_buffer.h>
```

Create a ring buffer over an existing array:

```c
int storage[4] = {0};
RingBuffer buffer = ringBufferCreate(storage, 4, sizeof(int));

int input = 10;
int output = 0;

bool pushed = buffer.vtable->push(&buffer, &input);
bool popped = buffer.vtable->pop(&buffer, &output);
```

Or let the ring buffer allocate its storage:

```c
RingBuffer buffer = ringBufferCreateDynamic(4, sizeof(int));

if (buffer.data == NULL) {
  /* Allocation failed. */
}
```

`push` returns `false` when the buffer is full or the input is invalid. `pop`
returns `false` when the buffer is empty or the output is invalid.

## How to clean it up

Always call `destroy` after using a dynamically allocated ring buffer:

```c
buffer.vtable->destroy(&buffer);
```

For a caller-owned buffer, `destroy` only clears the `RingBuffer` fields. The
caller's storage is not freed and remains the caller's responsibility.

## API summary

| Operation | Purpose |
| --- | --- |
| `ringBufferCreate` | Create a buffer over caller-owned storage. |
| `ringBufferCreateDynamic` | Create a buffer with allocated storage. |
| `push` | Copy one element to the back of the buffer. |
| `pop` | Copy and remove the oldest element. |
| `isEmpty` | Report whether the element count is zero. |
| `isFull` | Report whether the element count equals capacity. |
| `size` | Return the current element count. |
| `reset` | Reset indexes and count without freeing storage. |
| `destroy` | Free owned storage and invalidate the object. |

## Behavior and safety

- Elements are copied by value using the configured `elementSize`.
- When storing pointers, only pointer values are copied, not pointed-to data.
- The buffer does not overwrite old values when full.
- `reset` changes logical state; it does not erase the underlying bytes.
- The implementation is not thread-safe. Synchronize shared access externally.
- Use a non-null storage pointer, non-zero capacity, and non-zero element size.
