#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

static void *run(void *data) {
    printf("run\n");
    int *p = data;
    *p = 42;
    return NULL;
}

static void from_here() {
    int v;
    run(&v);
    printf("from_here: %d\n", v);
}

static void from_elsewhere() {
    pthread_t thread;
    int v;
    if (pthread_create(&thread, NULL, run, &v))
        exit(1);
    if (pthread_join(thread, NULL))
        exit(1);
    printf("from_elsewhere: %d\n", v);
}

int main() {
    from_here();
    from_elsewhere();
}
