#include "libfkae-shtae/controller/controller.h"
#include "libfkae-shtae/controller/controller_vtable.h"

#include <pthread.h>
#include <stdbool.h>

static bool controllerStart(void *controller) {

  Controller *this = (Controller *)controller;
  if (this == NULL) {
    goto catch;
  }

  pthread_mutex_lock(&this->mutex);
  this->running = true;
  pthread_mutex_unlock(&this->mutex);

  return true;

catch:
  return false;
}

static bool controllerStop(void *controller) {
  Controller *this = (Controller *)controller;
  if (this == NULL) {
    goto catch;
  }
  pthread_mutex_lock(&this->mutex);
  this->running = false;
  this->signaled = true;
  pthread_cond_signal(&this->condition);
  pthread_mutex_unlock(&this->mutex);
  return true;
catch:
  return false;
}

static bool controllerWait(void *controller) {
  Controller *this = (Controller *)controller;

  if (this == NULL) {
    goto catch;
  }

  pthread_mutex_lock(&this->mutex);

  while (this->running && !this->signaled) {
    pthread_cond_wait(&this->condition, &this->mutex);
  }

  pthread_mutex_unlock(&this->mutex);

  return true;

catch:
  return false;
}

static bool controllerIsRunning(void *controller) {
  Controller *this = (Controller *)controller;
  if (this == NULL) {
    goto catch;
  }
  pthread_mutex_lock(&this->mutex);
  bool running = this->running;
  pthread_mutex_unlock(&this->mutex);
  return running;

catch:
  return false;
}

static bool controllerDestroy(void *controller) {
  Controller *this = (Controller *)controller;
  if (this == NULL) {
    goto catch;
  }
  pthread_cond_destroy(&this->condition);
  pthread_mutex_destroy(&this->mutex);
  this->running = false;
  this->signaled = false;
  return true;
catch:
  return false;
}

static const ControllerVTable controllerVTable = {
    .start = controllerStart,
    .stop = controllerStop,
    .isRunning = controllerIsRunning,
    .wait = controllerWait,
    .destroy = controllerDestroy,
};

Controller controllerCreate(void) {
  Controller controller = {
      .vtable = &controllerVTable,
      .running = false,
      .signaled = false,
  };
  pthread_mutex_init(&controller.mutex, NULL);
  pthread_cond_init(&controller.condition, NULL);
  return controller;
}
