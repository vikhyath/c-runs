#include <stdio.h>
#include <stdlib.h>
#define SIZE 8

void str_rev(char *str, int size)
{
	int i = 0, j = size-1;
	if (str[0] == '-'){
		i = 1;
	}
	for (; i<=j; i++, j--) {
		char temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}
}

int itoarev(char *str, int x, int count)
{
	int remainder;
	while (x) {
		remainder = x % 10;
		str[count] = remainder + '0';
		x = x / 10;
		count++;
		if (count%SIZE == 0) {
			char *temp = realloc(str, count*2*sizeof(char));
			if (temp == NULL) {
				printf("unable to realloc memory");
				exit(0);
			}
		}
	}
	str[count] = '\0';
	return count;
}

int main(int c, char **argv)
{
	int x;
	printf("enter integer...\n");
	scanf("%d", &x);
	char *str = malloc(SIZE * sizeof(char));
	if (str == NULL) {
		printf("unable to allocate memory");
		exit(0);
	}
	int count = 0;
	if (x < 0) {
		str[count] = '-';
		count++;
		x *= -1;
	}

	count = itoarev(str, x, count);

	printf("before str rev %s\n", str);
	str_rev(str, count);
	printf("itoa is %s and sizeof int is %lu", str, sizeof(int));
	return 0;
}
