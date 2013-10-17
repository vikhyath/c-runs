#include <stdio.h>
#include <stdlib.h>

int* mergemerge(int*, int*, int ,int);

int* mergesplit(int array[], int low, int up)
{
	printf("got low: %d and up: %d\n", low, up);
	if (low == up) {
                printf("sending back %d\n",array[0]);
		return array;
	}
	int middle = (low+up)/2;
	int *arr1 = (int*)malloc(sizeof(int) * (middle+1 - low));
	int *arr2 = (int*)malloc(sizeof(int) * (up - middle));
	getchar();
	printf("size of arr1 is %lu and middle is %d and low is %d\n", sizeof(arr1), middle, low);
	printf("size of arr2 is %lu and up is %d\n", sizeof(arr2), up);
	int i = 0;
	while (i <= (middle-low)) {
                printf("copying(lower) %d\n",array[i]);

		arr1[i] = array[i];
		i++;
	}
	int j = (middle-low)+1; i=0;
	while (j <= up-middle+1) {
		arr2[i] = array[j];
                printf("copying(upper) %d\n",arr2[i]);
		j++;i++;
	}
	int *a = mergesplit(arr1, low, middle);
	int *b = mergesplit(arr2, middle+1, up);

	printf("size of a is %lu and 0th element is %d\n", sizeof(a), a[0]);
	printf("size of b is %lu and 0th element is %d\n", sizeof(b), b[0]);

	int *c = mergemerge(a, b, (middle+1-low), (up - middle));
        printf("in c, c[0]] is %d and c[1] is %d\n", c[0], c[1]);
	return c;
}

int* mergemerge(int left[], int right[], int r1, int r2)
{
	int l1 = 0, l2 = 0;
	int *arr = (int *)malloc(sizeof(int) * (r1+r2)); int index = 0;

	printf("r1 is %d and r2 is %d\n", r1, r2);
	while (l1 < r1) {
                printf("comparing %d with %d\n", left[l1], right[l2]);
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
				 printf("pushing in %d\n",arr[index]);
				 l1++; index++;
			}
		}

	}

	printf("(after merge)%d and %d\n", arr[0], arr[1]);
	return arr;
}

int main(int argc, char **argv)
{
	int a[] = {8,4,1,2,0,6,7};
        int size = sizeof(a)/sizeof(int);

	int *c = mergesplit(a, 0, sizeof(a)/sizeof(int)-1);

	printf("elements are %d %d %d %d %d %d %d", c[0], c[1], c[2], c[3], c[4], c[5], c[6]);
	return 0;
}
