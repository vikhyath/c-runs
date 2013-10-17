#include <stdio.h>
#include <pthread.h>

pthread_t threads[4];

void* search(int k)
{
	printf("%d\n", k);
}

int main(int argc, char **argv)
{
	int arr[100] = {0};
	int i;
	for (i = 0; i < 100; i++) {
		arr[i] = 1;
		//printf("%d\n", arr[i]);
	}

	for(i =0; i < 4; i++) {
		pthread_create(&threads[i], NULL, (void*)&search, (void *)i);
	}

	for(i = 0; i<4 ; i++) {
		pthread_join(threads[i], NULL);
	}
	return 0;
}
