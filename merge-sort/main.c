#include <stdio.h>
#include <stdlib.h>

int* mergemerge(int*, int*, int ,int);

int* mergesplit(int array[], int low, int up)
{
	if (low == up) {
		return array;
	}
	int middle = (low+up)/2;
	int *arr1 = (int*)malloc(sizeof(int) * (middle+1 - low));
	int *arr2 = (int*)malloc(sizeof(int) * (up - middle));
	int i = 0;
	while (i <= (middle)) {
		arr1[i] = array[i];
		i++;
	}
	int j = (middle)+1; int k=0;
	while (j <= up) {
		arr2[k] = array[j];
		j++;k++;
	}
	int *a = mergesplit(arr1, 0, i-1);
	int *b = mergesplit(arr2, 0, k-1);


	int *c = mergemerge(a, b, (middle+1-low), (up - middle));
	return c;
}

int* mergemerge(int left[], int right[], int r1, int r2)
{
	int l1 = 0, l2 = 0;
	int *arr = (int *)malloc(sizeof(int) * (r1+r2)); int index = 0;

	while (l1 < r1) {
		if (left[l1] < right[l2]) {
			arr[index] = left[l1];
			l1++;
		} else {
			arr[index] = right[l2];
			l2++;
		}
		index++;
		if (l1 == r1) {
			while (l2 < r2) {
				 arr[index] = right[l2];
				 l2++; index++;
			}
		} else if (l2 == r2) {
			while (l1 < r1) {
				 arr[index] = left[l1];
				 l1++; index++;
			}
		}

	}

	return arr;
}

int main(int argc, char **argv)
{
	int a[] = {8,4,1,2,0,6,7,-11,-12,99,-1000,15,13,80};

	int *c = mergesplit(a, 0, sizeof(a)/sizeof(int)-1);

	printf("elements are %d %d %d %d %d %d %d %d %d %d %d %d %d %d", c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], c[10], c[11], c[12], c[13]);
	return 0;
}
