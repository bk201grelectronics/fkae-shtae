# libfkae-shtae

A small reusable C utility library built for learning, low-level systems work, and cross-platform projects.

The library is intentionally designed around a few simple principles:

- C11
- fixed and predictable memory where practical
- small reusable components
- minimal dependencies
- simple `struct + vtable` object-style interfaces
- clear separation between public headers and implementation
- easy static-library integration with other C projects

The project currently contains a generic fixed-memory Ring Buffer. More reusable components can be added later under their own modules.

---

## Table of Contents

- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Build](#build)
- [Run Tests](#run-tests)
- [Install](#install)
- [Using the Library from Another Project](#using-the-library-from-another-project)
- [Uninstall](#uninstall)
- [Ring Buffer](#ring-buffer)

  - [What It Is](#what-it-is)
  - [Design](#design)
  - [Creating a Ring Buffer](#creating-a-ring-buffer)
  - [Push and Pop](#push-and-pop)
  - [Example: int](#example-int)
  - [Example: uint32_t](#example-uint32_t)
  - [Example: float](#example-float)
  - [Example: Struct](#example-struct)
  - [Example: Pointer Storage](#example-pointer-storage)
  - [Example: Fixed-Size String](#example-fixed-size-string)
  - [Wraparound Behavior](#wraparound-behavior)
  - [Ring Buffer API](#ring-buffer-api)
  - [Thread Safety](#thread-safety)

- [Adding New Components](#adding-new-components)

---

## Project Structure

```text
libfkae-shtae/
├── include/
│   └── libfkae-shtae/
│       └── buffer/
│           ├── constants.h
│           └── ring_buffer.h
├── src/
│   └── buffer/
│       └── ring_buffer.c
├── tests/
│   └── ring_buffer_test.c
├── Makefile
└── README.md
```

The important separation is:

```text
include/
    public API used by other projects

src/
    private implementation

tests/
    library tests

build/
    generated object files, test executables, and static library
```

Applications using this library should include headers from:

```c
#include <libfkae-shtae/buffer/ring_buffer.h>
```

---

## Requirements

The current build uses:

```text
Clang
Make
ar
C11
```

On macOS, the default Apple Clang toolchain is sufficient.

Verify:

```bash
clang --version
make --version
ar --version
```

---

## Build

Build the static library:

```bash
make
```

The build creates:

```text
build/
├── ring_buffer.o
└── libfkae-shtae.a
```

The build flow is:

```text
ring_buffer.c
      ↓
    clang
      ↓
ring_buffer.o
      ↓
      ar
      ↓
libfkae-shtae.a
```

`libfkae-shtae.a` is the reusable static library.

---

## Run Tests

Run:

```bash
make test
```

This first builds the library, then links the test against it:

```text
ring_buffer_test.c
        +
libfkae-shtae.a
        ↓
build/ring_buffer_test
        ↓
       run
```

---

## Install

Install the public headers and static library:

```bash
sudo make install
```

The default installation prefix is:

```text
/usr/local
```

After installation:

```text
/usr/local/
├── include/
│   └── libfkae-shtae/
│       └── buffer/
│           ├── constants.h
│           └── ring_buffer.h
└── lib/
    └── libfkae-shtae.a
```

The Makefile uses:

```makefile
PREFIX = /usr/local

INCLUDE_DIR = $(PREFIX)/include
LIB_DIR = $(PREFIX)/lib
```

So:

```text
$(INCLUDE_DIR)
```

becomes:

```text
/usr/local/include
```

and:

```text
$(LIB_DIR)
```

becomes:

```text
/usr/local/lib
```

The install process copies:

```text
include/libfkae-shtae/...
```

to:

```text
/usr/local/include/libfkae-shtae/...
```

and copies:

```text
build/libfkae-shtae.a
```

to:

```text
/usr/local/lib/libfkae-shtae.a
```

You can also choose another prefix:

```bash
sudo make install PREFIX=/opt/libfkae-shtae
```

which would install into:

```text
/opt/libfkae-shtae/include/
/opt/libfkae-shtae/lib/
```

---

## Using the Library from Another Project

Suppose another project contains:

```text
example/
├── main.c
└── Makefile
```

`main.c` can include the installed header:

```c
#include <libfkae-shtae/buffer/ring_buffer.h>
```

Compile with:

```bash
clang main.c \
  -I/usr/local/include \
  -L/usr/local/lib \
  -lfkae-shtae \
  -o app
```

The flags mean:

```text
-I/usr/local/include
    add /usr/local/include to the header search paths

-L/usr/local/lib
    add /usr/local/lib to the library search paths

-lfkae-shtae
    link against libfkae-shtae.a
```

Header lookup:

```text
#include <libfkae-shtae/buffer/ring_buffer.h>

                    ↓

search root:
/usr/local/include

                    +

libfkae-shtae/buffer/ring_buffer.h

                    ↓

/usr/local/include/libfkae-shtae/buffer/ring_buffer.h
```

Library lookup:

```text
-lfkae-shtae

      ↓

libfkae-shtae.a

      ↓

/usr/local/lib/libfkae-shtae.a
```

A simple Makefile for the consuming project could be:

```makefile
CC = clang

CFLAGS = -Wall -Wextra -std=c11 -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lfkae-shtae

all:
	$(CC) $(CFLAGS) main.c $(LDFLAGS) -o app

clean:
	rm -f app

.PHONY: all clean
```

If using clangd, generate a compilation database from the real build:

```bash
make clean
bear -- make
```

Then restart clangd so IntelliSense sees the same include paths as Clang.

---

## Uninstall

Remove the installed headers and library:

```bash
sudo make uninstall
```

This removes:

```text
/usr/local/include/libfkae-shtae/
/usr/local/lib/libfkae-shtae.a
```

Local build files can be removed with:

```bash
make clean
```

---

# Ring Buffer

## What It Is

A Ring Buffer is a fixed-capacity FIFO data structure.

FIFO means:

```text
First In
First Out
```

Conceptually:

```text
push → back of buffer

┌──────────────────────────────┐
│ A │ B │ C │   │   │   │   │
└──────────────────────────────┘

pop ← front of buffer
```

The buffer wraps around when an index reaches the end:

```text
0 → 1 → 2 → 3 → 0 → 1 → ...
```

This library's Ring Buffer is generic.

It can store any fixed-size C type because it only needs:

```text
storage address
capacity
element size
```

---

## Design

The public object contains:

```c
struct RingBuffer {
  const struct RingBufferVTable *vtable;

  void *data;

  size_t capacity;
  size_t elementSize;

  size_t readIndex;
  size_t writeIndex;
  size_t count;
};
```

The fields mean:

```text
data
    caller-provided storage

capacity
    maximum number of elements

elementSize
    size of one element in bytes

readIndex
    position of the next pop

writeIndex
    position of the next push

count
    number of elements currently stored
```

The Ring Buffer does not allocate its own element storage.

The caller owns the storage.

Example:

```c
int storage[4];

RingBuffer buffer =
    ringBufferCreate(
        storage,
        4,
        sizeof(int)
    );
```

This gives:

```text
capacity    = 4
elementSize = sizeof(int)
data        = storage
```

---

## Creating a Ring Buffer

The general pattern is:

```c
Type storage[CAPACITY];

RingBuffer buffer =
    ringBufferCreate(
        storage,
        CAPACITY,
        sizeof(Type)
    );
```

The Ring Buffer does not need to know what `Type` is.

Internally, the location of an element is calculated using:

```text
address = base + index × elementSize
```

For example, if:

```text
writeIndex = 3
elementSize = 4
```

then:

```text
offset = 3 × 4
       = 12 bytes
```

---

## Push and Pop

Push:

```c
buffer.vtable->push(&buffer, &value);
```

Pop:

```c
buffer.vtable->pop(&buffer, &output);
```

The addresses are passed because the generic Ring Buffer operates on raw memory.

Push conceptually performs:

```text
item
 ↓
copy elementSize bytes
 ↓
storage[writeIndex]
 ↓
advance writeIndex
 ↓
count++
```

Pop performs:

```text
storage[readIndex]
 ↓
copy elementSize bytes
 ↓
output
 ↓
advance readIndex
 ↓
count--
```

---

## Example: int

```c
#include <stdio.h>

#include <libfkae-shtae/buffer/ring_buffer.h>

int main(void) {
  int storage[4];

  RingBuffer buffer =
      ringBufferCreate(
          storage,
          4,
          sizeof(int)
      );

  int input = 42;

  if (!buffer.vtable->push(&buffer, &input)) {
    printf("push failed\n");
    return 1;
  }

  int output = 0;

  if (!buffer.vtable->pop(&buffer, &output)) {
    printf("pop failed\n");
    return 1;
  }

  printf("output = %d\n", output);

  return 0;
}
```

Expected output:

```text
output = 42
```

---

## Example: uint32_t

```c
#include <stdint.h>

#include <libfkae-shtae/buffer/ring_buffer.h>

int main(void) {
  uint32_t storage[8];

  RingBuffer buffer =
      ringBufferCreate(
          storage,
          8,
          sizeof(uint32_t)
      );

  uint32_t input = 500;
  uint32_t output = 0;

  buffer.vtable->push(&buffer, &input);
  buffer.vtable->pop(&buffer, &output);

  return 0;
}
```

For a 32-bit integer:

```text
elementSize = 4 bytes
```

so the storage offsets are:

```text
index 0 → byte 0
index 1 → byte 4
index 2 → byte 8
index 3 → byte 12
...
```

---

## Example: float

A Ring Buffer can hold audio samples directly:

```c
#include <libfkae-shtae/buffer/ring_buffer.h>

int main(void) {
  float storage[1024];

  RingBuffer buffer =
      ringBufferCreate(
          storage,
          1024,
          sizeof(float)
      );

  float sample = 0.75f;

  buffer.vtable->push(
      &buffer,
      &sample
  );

  return 0;
}
```

---

## Example: Struct

The Ring Buffer can also store complete structs.

Example audio frame:

```c
typedef struct {
  float speaker1;
  float speaker2;
  float speaker3;
} AudioFrame;
```

Create storage:

```c
AudioFrame storage[1024];

RingBuffer buffer =
    ringBufferCreate(
        storage,
        1024,
        sizeof(AudioFrame)
    );
```

Push one complete frame:

```c
AudioFrame frame = {
    .speaker1 = 0.5f,
    .speaker2 = 0.8f,
    .speaker3 = 0.2f,
};

buffer.vtable->push(
    &buffer,
    &frame
);
```

The whole struct is copied as one Ring Buffer element:

```text
Frame N

[Speaker 1][Speaker 2][Speaker 3]
```

This is useful when several values belong to the same logical instant or event.

---

## Example: Pointer Storage

A Ring Buffer can store pointers:

```c
char *storage[8];

RingBuffer buffer =
    ringBufferCreate(
        storage,
        8,
        sizeof(char *)
    );
```

Example:

```c
char *message = "hello";

buffer.vtable->push(
    &buffer,
    &message
);
```

Important:

```text
the pointer value is copied

the string contents are NOT copied
```

So the Ring Buffer contains something conceptually like:

```text
[0x12345678]
```

rather than:

```text
[h][e][l][l][o]
```

The memory referenced by the pointer must remain valid while the pointer is stored in the buffer.

---

## Example: Fixed-Size String

If the Ring Buffer should own a copied fixed-size string value, use a fixed-size type.

```c
typedef struct {
  char value[64];
} String64;
```

Then:

```c
String64 storage[8];

RingBuffer buffer =
    ringBufferCreate(
        storage,
        8,
        sizeof(String64)
    );
```

Push:

```c
String64 text = {
    .value = "hello"
};

buffer.vtable->push(
    &buffer,
    &text
);
```

Now each Ring Buffer element is the full fixed-size `String64` value.

---

## Wraparound Behavior

Suppose:

```text
capacity = 4
```

Indices progress as:

```text
0 → 1 → 2 → 3 → 0
```

The implementation uses:

```c
(this->writeIndex + 1) % this->capacity
```

and:

```c
(this->readIndex + 1) % this->capacity
```

Example:

```text
push 10
push 20
push 30
push 40

buffer full
```

Then:

```text
pop → 10
pop → 20
```

The first two slots become logically available.

Now:

```text
push 50
push 60
```

The write index wraps around:

```text
physical storage:

[50][60][30][40]
```

but FIFO order is still:

```text
30
40
50
60
```

because `readIndex`, `writeIndex`, and `count` define the logical queue state.

---

## Ring Buffer API

Create:

```c
RingBuffer ringBufferCreate(
    void *storage,
    size_t capacity,
    size_t elementSize
);
```

Push:

```c
bool push(
    RingBuffer *this,
    const void *item
);
```

Returns:

```text
true  → item written
false → buffer full or invalid input
```

Pop:

```c
bool pop(
    RingBuffer *this,
    void *item
);
```

Returns:

```text
true  → item read
false → buffer empty or invalid input
```

Check whether empty:

```c
bool isEmpty(
    RingBuffer *this
);
```

Check whether full:

```c
bool isFull(
    RingBuffer *this
);
```

Get current number of stored elements:

```c
size_t size(
    RingBuffer *this
);
```

Reset logical state:

```c
void reset(
    RingBuffer *this
);
```

Reset sets:

```text
readIndex  = 0
writeIndex = 0
count      = 0
```

The caller-provided storage does not need to be physically zeroed because the Ring Buffer's logical state determines which elements are valid.

---

## Thread Safety

The current `RingBuffer` is intentionally **not thread-safe**.

It is suitable when access is externally synchronized or owned by one thread.

Thread safety should be explicit rather than automatically added to every container.

A future concurrency module can provide a separate:

```text
SpscRingBuffer
```

for:

```text
one producer thread
+
one consumer thread
```

which is particularly useful for low-latency pipelines.

Keeping these as separate types avoids adding mutex or atomic overhead to the basic Ring Buffer.

---

# Adding New Components

Each reusable component should get its own public header and implementation folder.

For example:

```text
include/
└── libfkae-shtae/
    ├── buffer/
    │   └── ring_buffer.h
    ├── concurrency/
    │   └── cancel_token.h
    ├── protocol/
    │   └── ndjson_buffer.h
    └── containers/
        └── array.h
```

with corresponding implementations:

```text
src/
├── buffer/
├── concurrency/
├── protocol/
└── containers/
```

Each component should also get its own README section covering:

```text
what it is
why it exists
API
memory ownership
thread-safety guarantees
usage examples
tests
```

The goal is to keep `libfkae-shtae` small, understandable, and reusable across my unrelated fkae-shtae C projects.
