#include <stdio.h>
#include <stdlib.h>

void swap(int *arr, int i, int j)
{
	int temp = arr[j];
	arr[j] = arr[i];
	arr[i] = temp;
}

int partition(int *arr, int start, int end)
{
	int i = start - 1;
	int j;
	for(j = start; j < end; j++) {
		if (arr[j] > arr[end]) {
			//i++;
		} else {
			i++;
			swap(arr, i, j);
		}
	}
	int loc = i+1;
	swap(arr, loc, end);
	return loc;
}

void quick_sort(int *arr, int start, int end)
{
	if (start >= end) {
		return;
	}
	int x = partition(arr, start, end);
	quick_sort(arr, start, x-1);
	quick_sort(arr, x+1, end);
}

int main(int argc, char **argv)
{
	int arr[] = {5,3,1,7,8, -23, -772, 1, 5, 1, 8, 2, 10, 4, 14, -19, -24, -25};
	int size = sizeof(arr)/sizeof(int);
	quick_sort(arr, 0, size-1);

	printf("\n");
	int i;
	for (i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	return 0;
}
