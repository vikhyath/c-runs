#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int intersearch(int *arr, int left, int right, int *tries, int search)
{
	(*tries)++;
	if (left == right) {
		if (arr[left] == search) {
			return left;
		}
		else {
			return -1;
		}
	}

	// get an estimate of approximate which fraction of the array the search element is in
	// calculate the fraction and then multiply the fraction with the range + left to get the index to check
	float fraction = (float)(search - arr[left])/(arr[right] - arr[left]);
	if (fraction < 0 || fraction > 1) {
		return -1;
	}

	int estimate = left + round(fraction * (right - left));

	if (arr[estimate] > search) {
		right = estimate - 1;
	} else if (arr[estimate] < search) {
		left = estimate + 1;
	} else {
		return estimate;
	}
	return intersearch(arr, left, right, tries, search);
}

int main(int argc, char **argv)
{
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100, 200, 300, 1000, 2000, 3000};
	int right = sizeof(arr)/sizeof(arr[0]) - 1;
	int left = 0, tries = 0;
	int searchfor = 2000;

	int ret = intersearch(arr, left, right, &tries, searchfor);
	printf("found at index %d with %d tries\n", ret, tries);
	return 0;
}