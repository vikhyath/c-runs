#include <stdio.h>
#include <time.h>

void swap(int*, int*);

int main(int argc, char **argv)
{
	int arr[] = {1,2,3,4,5,6,7,8};
	int size = sizeof(arr)/sizeof(arr[0]);

	printf("size is %d\n", size);
	int i = 0;
	srand(time(0));
	for (i = 0; i < size; i++)
	{
		swap(&arr[i], &arr[rand()%size]);
	}

	for (i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}

	return 0;
}

void swap(int *a, int *b)
{
	int c;
	c = *b;
	*b = *a;
	*a = c;
}