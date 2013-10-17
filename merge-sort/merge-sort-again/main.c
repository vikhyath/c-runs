#include <stdio.h>
#include <stdlib.h>

int *mergemerge(int *, int *, int, int);
int *mergesplit(int arr[], int start, int end)
{
	if (end-start == 1) return arr;

	int mid = (end+start)/2;
	int *lower = malloc(sizeof(int) * (mid-start)); //copy upto mid, including start
	int *upper = malloc(sizeof(int) * (end-mid));	// copy upto end, including mid

	int s = 0;
	while (start < mid) {
		lower[s++] = arr[start++];
	}
	int m = 0;
	while (mid < end) {
		upper[m++] = arr[mid++];
	}

	int *lowersorted = mergesplit(lower, 0, s);
	int *uppersorted = mergesplit(upper, 0, m);

	int *merged = mergemerge(lowersorted, uppersorted, s, m);
	int retsize = sizeof(merged)/sizeof(*merged);

	return merged;
}

int* mergemerge(int *lower, int *upper, int lowersize, int uppersize)
{
	int size = uppersize+lowersize;
	int *mergedarray = malloc(sizeof(int) * size);

	int l1 = 0, l2 = 0, count = 0;
	while (count < size) {
		if (lower[l1] <= upper[l2]) {
			mergedarray[count++] = lower[l1++];
		} else {
			mergedarray[count++] = upper[l2++];
		}
		if (l1 == lowersize) {
			while (l2 < uppersize) {
				mergedarray[count++] = upper[l2++];
			}
		}
		if (l2 == uppersize) {
			while (l1 < lowersize) {
				mergedarray[count++] = lower[l1++];
			}
		}
	}

	return mergedarray;
}

int main(int argc, char **argv)
{
	int arr[] = {5,3,12,56, -1, 2, 213, -23, 9, 90, 65, 33, 92, -89};
	int size = sizeof(arr)/sizeof(*arr);

	int *ret = mergesplit(arr, 0, size);

	int retsize = sizeof(ret)/sizeof(*ret);

	int i;
	for (i = 0; i < size; i++) {
		printf("%d ", ret[i]);
	}
	printf("\n");
}