#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 10000

int global = 0;
pthread_mutex_t mutex;

void* start(void *x)
{
	/*int *i = x;
	*i += 1;
	printf("Thread %d\n", *i);*/
	pthread_mutex_lock(&mutex);
	global++;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main(int argc, char **argv) {
	pthread_t thread[THREADS];
	int arr[10000] = {[0 ... 9999] = 1};
	int i = 0;
	int ret = pthread_mutex_init(&mutex, NULL);
	if (ret) {
		exit(0);
	}
	for (; i < THREADS; i++) {
		ret = pthread_create(&thread[i], NULL, (void *)start, (void *)&arr[i]);
		if (ret) {
			exit(0);
		}
	}

	for (i = 0; i < THREADS; i++) {
		pthread_join(thread[i], NULL);
	}

	for (i =0; i < THREADS; i++) {
		//printf("%d\n", arr[i]);
	}
	printf("global is %d\n", global);
	return 0;
}