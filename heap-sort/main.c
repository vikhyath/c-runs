#include <stdio.h>
#include <stdlib.h>

void build_heap(int *, int);
void heapify(int *, int, int);
void swap(int *, int, int);

int main(int argc, char **argv)
{
	int arr[] = {5,2,0,3,8,4,-1, 99, 1000, -5, 44, 6, 2, -44, 1, 0, 9, 11, 8, 78, 7};
	int heapsize = sizeof(arr)/sizeof(int);
	int length = heapsize;
	printf("heap size: %d\n", heapsize);

	build_heap(arr, heapsize);

	while (heapsize >= 2) {
		swap(arr, 1, heapsize-1);
		heapsize--;
		heapify(arr, 1, heapsize);
	}

	int i;
	printf("\n");
	for (i = 0; i < length; i++) {
		printf("%d ", arr[i]);
	}
	return 1;
}

void build_heap(int arr[], int heapsize)
{
	int k;
	for (k=heapsize/2; k >= 1; k--) {
		heapify(arr, k, heapsize);
	}
}

void heapify(int arr[], int k, int heapsize)
{
	int max;
	if (2*k-1 < heapsize && arr[2*k-1] > arr[k-1]) {
		max = 2*k-1;
	} else {
		max = k-1;
	}
	if (2*k < heapsize && arr[2*k] > arr[max]) {
		max = 2*k;
	}
	if (max != k-1) {
		swap(arr, k, max);
		heapify(arr, max+1, heapsize);
	}
}

void swap(int arr[], int k, int max)
{
	int temp = arr[k-1];
	arr[k-1] = arr[max];
	arr[max] = temp;
}
