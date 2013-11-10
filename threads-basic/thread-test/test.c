#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THREADS 10000

int global = 0;
pthread_mutex_t mutex;

void* increment(void *i)
{
    //int *k = i;
    //printf("thread %d reached here\n", *k);
    pthread_mutex_lock(&mutex);
    global++;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t thread[THREADS];
    int params[THREADS] = {[0 ... THREADS-1] = 1};
    pthread_mutex_init(&mutex, NULL);

    int i = 0;
    for (; i < THREADS; i++) {
        int ret = pthread_create(&thread[i], NULL, (void *)increment, (void *)&params[i]);
        if (ret) {
            printf("thread creation failed\n");
        }
    }

    for (i = 0; i < THREADS; i++) {
        pthread_join(thread[i], NULL);
    }

    printf("global value is %d\n", global);

    return 0;
}