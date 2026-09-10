#pragma once

#include "libfkae-shtae/controller/controller_vtable.h"

#include "pthread.h"

typedef struct Controller Controller;

struct Controller {
  const ControllerVTable *vtable;

  pthread_mutex_t mutex;
  pthread_cond_t condition;

  bool running;
  bool signaled;
};

Controller controllerCreate(void);
