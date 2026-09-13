
# Install and use the library

This guide shows the two installation methods. Each method creates an empty C
project and uses the library from both the terminal and a Makefile.

## Table of contents

- [Example program](#example-program)
- [Method 1: install with sudo](#method-1-install-with-sudo)
- [Method 2: install inside a project](#method-2-install-inside-a-project)
- [Compiler options](#compiler-options)

## Example program

Both methods use this `main.c` file:

```c
#include <stdio.h>

#include <libfkae-shtae/buffer/ring_buffer.h>

int main(void) {
  int storage[4] = {0};
  RingBuffer buffer = ringBufferCreate(storage, 4, sizeof(int));

  int input = 42;
  int output = 0;

  buffer.vtable->push(&buffer, &input);
  buffer.vtable->pop(&buffer, &output);

  printf("Output: %d\n", output);
  buffer.vtable->destroy(&buffer);
  return 0;
}
```

This line imports the public ring buffer declarations:

```c
#include <libfkae-shtae/buffer/ring_buffer.h>
```

The compiler must also link the program with `libfkae-shtae.a`.

## Method 1: install with sudo

This installs the library under `/usr/local` for all local projects.

### 1. Install the library

Run from the `libfkae-shtae` repository:

```bash
make clean
make test
sudo make install
```

The headers are installed under `/usr/local/include`. The static library is
installed under `/usr/local/lib`.

### 2. Create an empty project

```bash
mkdir system-example
cd system-example
touch main.c Makefile
```

Copy the [example program](#example-program) into `main.c`.

### 3. Build the system example from the terminal

```bash
clang -Wall -Wextra -std=c11 main.c \
  -I/usr/local/include \
  -L/usr/local/lib \
  -lfkae-shtae \
  -o app
```

Run it:

```bash
./app
```

Expected output:

```text
Output: 42
```

### 4. Build the system example with a Makefile

Put this in `Makefile`:

```makefile
CC = clang
CFLAGS = -Wall -Wextra -std=c11
CPPFLAGS = -I/usr/local/include
LDFLAGS = -L/usr/local/lib
LDLIBS = -lfkae-shtae

app: main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) main.c $(LDFLAGS) $(LDLIBS) -o app

clean:
	rm -f app

.PHONY: clean
```

Build and run:

```bash
make
./app
```

## Method 2: install inside a project

This installs the library inside one project without `sudo`. Start with the
library repository and the new project beside each other.

### 1. Create an empty project

From the directory containing `libfkae-shtae`, run:

```bash
mkdir local-example
cd local-example
touch main.c Makefile
cd ..
```

Copy the [example program](#example-program) into `local-example/main.c`.

### 2. Install the library into the project

```bash
cd libfkae-shtae
make clean
make test
make install PREFIX="$(cd ../local-example && pwd)/vendor/libfkae-shtae"
cd ../local-example
```

The library is now under `local-example/vendor/libfkae-shtae`.

### 3. Build the local example from the terminal

Run from `local-example`:

```bash
clang -Wall -Wextra -std=c11 main.c \
  -Ivendor/libfkae-shtae/include \
  -Lvendor/libfkae-shtae/lib \
  -lfkae-shtae \
  -o app
```

Build and run:

```bash
./app
```

Expected output:

```text
Output: 42
```

### 4. Build the local example with a Makefile

Put this in `local-example/Makefile`:

```makefile
CC = clang
CFLAGS = -Wall -Wextra -std=c11
LIBFKAE = vendor/libfkae-shtae
CPPFLAGS = -I$(LIBFKAE)/include
LDFLAGS = -L$(LIBFKAE)/lib
LDLIBS = -lfkae-shtae

app: main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) main.c $(LDFLAGS) $(LDLIBS) -o app

clean:
	rm -f app

.PHONY: clean
```

Build and run:

```bash
make
./app
```

## Compiler options

| Option           | Meaning                                 |
| ---------------- | --------------------------------------- |
| `-I`             | Where Clang finds the library headers.  |
| `-L`             | Where Clang finds the compiled library. |
| `-lfkae-shtae`   | Link with`libfkae-shtae.a`.             |
| `-o app`         | Create an executable named`app`.        |
