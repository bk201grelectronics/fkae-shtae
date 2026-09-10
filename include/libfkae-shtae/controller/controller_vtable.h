#pragma once

#include "stdbool.h"

typedef struct ControllerVTable ControllerVTable;

struct ControllerVTable {
  bool (*start)(void *this);
  bool (*stop)(void *this);
  bool (*isRunning)(void *this);
  bool (*wait)(void *this);
  bool (*destroy)(void *this);
};
