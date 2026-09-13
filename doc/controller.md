# Controller

## Table of contents

- [What it is](#what-it-is)
- [Build status](#build-status)
- [How to use it](#how-to-use-it)
- [How to clean it up](#how-to-clean-it-up)
- [API summary](#api-summary)
- [Behavior and safety](#behavior-and-safety)

## What it is

`Controller` is a small POSIX threads synchronization helper. It owns a mutex
and condition variable and exposes start, stop, state-check, wait, and destroy
operations through a vtable.

## Build status

The controller is currently source-only. Its header and implementation are in
the repository, but `src/controller/controller.c` is not included in the
Makefile or static archive. Integrate and test it before treating it as an
installed feature.

## How to use it

```c
#include <libfkae-shtae/controller/controller.h>

Controller controller = controllerCreate();

controller.vtable->start(&controller);

if (controller.vtable->isRunning(&controller)) {
  /* Start or continue controlled work. */
}

controller.vtable->stop(&controller);
```

`wait` blocks while the controller is running and has not been signaled.
`stop` sets the running state to false and signals one waiter.

## How to clean it up

Destroy the controller only after all threads have stopped using it:

```c
controller.vtable->destroy(&controller);
```

This destroys the owned POSIX mutex and condition variable. A destroyed
controller must not be used again.

## API summary

| Operation | Purpose |
| --- | --- |
| `controllerCreate` | Initialize a controller and synchronization objects. |
| `start` | Set the running state to true. |
| `stop` | Stop the controller and signal one waiter. |
| `isRunning` | Read the running state under the mutex. |
| `wait` | Wait until stopped or signaled. |
| `destroy` | Destroy the condition variable and mutex. |

## Behavior and safety

- This component depends on POSIX threads and is not portable to every C11
  platform without adaptation.
- Do not copy a live `Controller`; it contains initialized synchronization
  objects.
- Coordinate thread shutdown before calling `destroy`.
- The current signal state is one-way: `start` does not clear `signaled`.
