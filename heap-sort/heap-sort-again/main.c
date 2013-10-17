#include <stdio.h>
#include <stdlib.h>

void swap(int *arr, int i, int j)
{
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

void heapify(int *arr, int x, int heapsize)
{
	int max = x;
	if ((2*x)+1 < heapsize && arr[(2*x)+1] > arr[max]) {
		max = 2*x+1;
	}
	if ((2*x)+2 < heapsize && arr[(2*x)+2] > arr[max]) {
		max = 2*x+2;
	}

	if (max != x) {
		swap(arr, x, max);
		heapify(arr, max, heapsize);
	}
}

int main(int argc, char **argv)
{
	int arr[] = {5,2,0,3,8,4,-1, 99, 1000, -5, 44, 6, 2, -44, 1, 0, 9, 11, 8, 78, 7};
	int heapsize = sizeof(arr) / sizeof(*arr);
	int copy = heapsize;

	int i = (heapsize-1) / 2;

	for (; i >= 0; i--) {
		heapify(arr, i, heapsize);
	}

	while (heapsize > 1) {
		heapsize--;
		swap(arr, 0, heapsize);
		heapify(arr, 0, heapsize);
	}

	i = 0;
	for (; i < copy; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}