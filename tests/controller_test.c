#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "libfkae-shtae/controller/controller.h"

static void *worker(void *argument) {
  Controller *controller = (Controller *)argument;

  printf("Worker: Waiting...\n");
  assert(controller->vtable->wait(controller) == true);
  printf("Worker: Woke up\n");
  return NULL;
}

int main(void) {
  Controller controller = controllerCreate();
  pthread_t thread;

  assert(controller.vtable->start(&controller) == true);
  assert(controller.vtable->isRunning(&controller) == true);

  assert(pthread_create(&thread, NULL, worker, &controller) == 0);

  sleep(4);

  printf("Main: stopping controller");

  assert(controller.vtable->stop(&controller) == true);

  assert(pthread_join(thread, NULL) == 0);

  assert(controller.vtable->isRunning(&controller) == false);

  assert(controller.vtable->destroy(&controller) == true);

  printf("Controller wait/signal test passed\n");

  return 0;
}
