#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

static pthread_mutex_t mutex;

static void mutex_init(void) {
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
}

static void *run(void *data) {
    printf("run\n");
    pthread_mutex_lock(&mutex);

    int *p = data;
    *p = 42;

    pthread_mutex_unlock(&mutex);
    return NULL;
}

static void from_here() {
    pthread_mutex_lock(&mutex);

    int v;
    run(&v);
    printf("from_here: %d\n", v);

    pthread_mutex_unlock(&mutex);
}

static void from_elsewhere() {
    pthread_mutex_lock(&mutex);

    pthread_t thread;
    int v;
    if (pthread_create(&thread, NULL, run, &v))
        exit(1);
    if (pthread_join(thread, NULL))
        exit(1);
    printf("from_elsewhere: %d\n", v);

    pthread_mutex_unlock(&mutex);
}

int main() {
    mutex_init();
    from_here();
    from_elsewhere();
    pthread_mutex_destroy(&mutex);
}
