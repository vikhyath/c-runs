/*
You have two integer arrays. Treat these arrays as if they were big numbers,
with one digit in each slot. Perform addition on these two arrays and store the results in a new array.
*/

#include <stdio.h>
#include <stdlib.h>

int *sum(int *a, int *b, int asize, int bsize, int maxsize)
{
    int *result = malloc((maxsize+1) * sizeof(*a));
    int carry = 0;
    while (maxsize > 0) {
        int sum = carry;
        if (asize >= 0) {
            sum += a[asize--];
        }
        if (bsize >= 0) {
            sum += b[bsize--];
        }
        carry = sum / 10;
        result[maxsize--] = sum % 10;
    }
    result[0] = carry;
    return result;
}

int main(int argc, char **argv)
{
    int a[] =          {1, 2, 4, 5, 6, 7};
    int b[] = {2, 1, 9, 9, 3, 3, 2, 1, 9};
    int asize = sizeof(a) / sizeof(*a);
    int bsize = sizeof(b) / sizeof(*b);
    int maxsize;

    if (asize > bsize) {
        maxsize = asize;
    } else {
        maxsize = bsize;
    }

    int *result = sum(a, b, asize - 1, bsize - 1, maxsize);
    int i = 0;
    for (; i <= maxsize; i++) {
        printf("%d", result[i]);
    }
    printf("\n");
    return 0;
}