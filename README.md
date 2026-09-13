# libfkae-shtae

`libfkae-shtae` is a small C11 utility library for learning, low-level systems
work, and reuse across C projects. It favors predictable memory use, minimal
dependencies, caller-owned storage, and simple `struct + vtable` interfaces.

## Table of contents

- [What is in the library?](#what-is-in-the-library)
- [Install the library](#install-the-library)
- [Use the library](#use-the-library)
- [Feature documentation](#feature-documentation)
- [Development](#development)
- [Add a new component](#add-a-new-component)
- [Uninstall the library](#uninstall-the-library)

## What is in the library?

The library provides reusable containers and synchronization helpers. Public
headers live under `include/libfkae-shtae/`; implementations live under `src/`.

The current static library contains the ring buffer, byte bitmap, and array.
The hash map and controller are present in source form but are not yet included
in the Makefile build or installed archive.

## Install the library

### Requirements

You need a C11 compiler, Make, and `ar`. The current Makefile uses Clang.

```bash
clang --version
make --version
ar --version
```

### Build and test first

```bash
make
make test
```

This produces `build/libfkae-shtae.a` and runs the tests for components included
in the archive.

### Install with the default prefix

```bash
sudo make install
```

The default prefix is `/usr/local`, so the command installs:

```text
/usr/local/include/libfkae-shtae/
/usr/local/lib/libfkae-shtae.a
```

### Install with a custom prefix

Use a custom prefix when you do not want to install under `/usr/local`:

```bash
make install PREFIX="$PWD/local"
```

This installs the headers under `local/include` and the archive under
`local/lib`. A user-writable prefix normally does not require `sudo`.

## Use the library

Include a component header and link the executable with `libfkae-shtae.a`.
The following example creates a fixed-capacity ring buffer with caller-owned
storage:

```c
#include <stdio.h>

#include <libfkae-shtae/buffer/ring_buffer.h>

int main(void) {
  int storage[4] = {0};
  RingBuffer buffer = ringBufferCreate(storage, 4, sizeof(int));

  int input = 42;
  int output = 0;

  if (!buffer.vtable->push(&buffer, &input) ||
      !buffer.vtable->pop(&buffer, &output)) {
    return 1;
  }

  printf("%d\n", output);
  buffer.vtable->destroy(&buffer);
  return 0;
}
```

After installing under `/usr/local`, compile it with:

```bash
clang -std=c11 main.c \
  -I/usr/local/include \
  -L/usr/local/lib \
  -lfkae-shtae \
  -o app
```

For a custom prefix, replace both `/usr/local` paths with that prefix. When
linking directly from this checkout instead, use:

```bash
clang -std=c11 main.c -Iinclude build/libfkae-shtae.a -o app
```

Each component guide explains storage ownership and its cleanup method. Calling
`destroy` on an object backed by caller-owned storage invalidates the library
object but does not free the caller's array.

## Feature documentation

- [Complete installation and usage examples](doc/examples.md)
- [Ring buffer](doc/buffer.md)
- [Byte bitmap](doc/bitmap.md)
- [Array](doc/array.md)
- [Hash map](doc/hash-map.md)
- [Controller](doc/controller.md)

## Development

Build and run the C tests:

```bash
make test
```

Install the documentation tooling and lint all Markdown files:

```bash
npm install
npm run lint:markdown
```

The Markdown linter is a local development dependency. It does not become part
of the C library or its installed files. You can also run it through Make:

```bash
make lint-markdown
```

Remove generated C build files with:

```bash
make clean
```

## Add a new component

Keep each new feature in its own module and follow this layout:

```text
include/libfkae-shtae/<module>/<interface>.h
include/libfkae-shtae/<module>/<component>.h
src/<module>/<component>.c
tests/<component>_test.c
doc/<component>.md
```

Use this checklist as the contribution template:

1. Define the generic vtable interface in `<interface>.h` when one is needed.
2. Define the public concrete type and constructors in `<component>.h`.
3. Put private helper functions and the implementation in `<component>.c`.
4. State who owns every storage allocation and how cleanup works.
5. Add the object to `libfkae-shtae.a` in the Makefile.
6. Add a test target and include it in the main `test` target.
7. Add `doc/<component>.md` with this table of contents:
   - What it is
   - How to use it
   - How to clean it up
   - API summary
   - Safety or thread-safety notes
8. Add the new guide to [Feature documentation](#feature-documentation).
9. Run `make test` and `npm run lint:markdown` before submitting the change.

## Uninstall the library

Use the same prefix that was used during installation:

```bash
sudo make uninstall
```

For a custom prefix:

```bash
make uninstall PREFIX="$PWD/local"
```
