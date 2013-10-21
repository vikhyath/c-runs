/*
Program to do to n-way merge, where n is the SIZE of n*n array
Emulating n-way merge from file through arrays, where each array row corresponds to a sorted file

Running time: O(n^2 logn), will be O(n^3) if min heap is not used

1) Get sizeof(min-heap) elements from n*n array
2) Put in min heap, remove the root element O(log n)
3) Fill min heap with a new element from array O(#elements) = O(n^2)
4) Do steps 2-3 till n*n array is exhausted
5) Now remove the root from min heap as many times as there are elements left in heap
*/

#include <stdio.h>
#include <stdlib.h>
#define SIZE 5

void printheap(int *heap)
{
	int i;
	for(i = 0; i < SIZE; i++) {
		printf("heap: %d\n", heap[i]);
	}
}

void swap(int *heap, int a, int b)
{
	// no extra space for swapping
	heap[a] ^= heap[b];
	heap[b] ^= heap[a];
	heap[a] ^= heap[b];
}

void heapify(int *heap, int parent, int heapsize)
{
	// restrict heapsize if more than SIZE
	if (heapsize >= SIZE) {
		heapsize = SIZE;
	}

	int min = parent;
	int leftchild = (2 * parent) + 1;
	int rightchild = (2 * parent) + 2;

	if (leftchild < heapsize && heap[leftchild] < heap[min]) {
		min = leftchild;
	}
	if (rightchild < heapsize && heap[rightchild] < heap[min]) {
		min = rightchild;
	}
	
	// min changed, so swap
	if (min != parent) {
		swap(heap, min, parent);
		heapify(heap, min, heapsize);
	}
}

int getelement(int (*arr)[SIZE], int *row, int *col)
{
	int x = arr[*row][*col];
	(*row)++;
	if ((*row) % SIZE == 0) {
		(*col)++;
		(*row) = 0;
	}
	return x;
}

int main(int argc, char **argv)
{
	int arr[SIZE][SIZE] = {2, 8, 12, 16, 32, 4, 5, 9, 18, 36, 7, 10, 14, 28, 56, 1, 5, 10, 20, 40, 3, 8, 19, 38, 60};
	int row = 0, col = 0;
	int heap[SIZE];
	int mhcount = 0;
	int elementsleft = SIZE*SIZE;

	for (mhcount = 0; mhcount < SIZE; mhcount++) {
		heap[mhcount] = getelement(arr, &row, &col);

	}

	// heapify from n/2th-1, the first possible parent to have children
	mhcount = SIZE/2 - 1;
	while (mhcount >= 0) {
		heapify(heap, mhcount, SIZE);
		mhcount--;
	}

	// while there are elements left in array which can be added to heap
	while (elementsleft > SIZE) {
		printf("%d\n", heap[0]);
		heap[0] = getelement(arr, &row, &col);
	    elementsleft--;
		heapify(heap, 0, elementsleft);
	}

	// get remaining elements out of heap
	for (mhcount = 0; mhcount < SIZE; mhcount++) {
		printf("%d\n", heap[0]);
		swap(heap, 0, elementsleft-1);
		elementsleft--;
		heapify(heap, 0, elementsleft);
	}
	return 0;
}