#include <stdio.h>

int main(int argc, char **argv)
{
	int a[] = {3, 2, 5, 10, -1, -4, 9, -100, 99, 12314, 1, 2, 0, -5, 8};
	int size = sizeof(a)/sizeof(int), i;

	for (i = 1; i < size; i++) {
		int pos = i, val = a[i];
		while (pos > 0 && val < a[pos-1]) {
			a[pos] = a[pos-1];
			pos--;
		}
		a[pos] = val;
	}

	for (i=0; i<size; i++) {
		printf("%d\n", a[i]);
	}
	return 0;
}
