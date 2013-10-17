#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char* removedups(char *a)
{
	char *op = malloc(10*sizeof(char));
	bool hashmap[256] = {0};
	int count = 0;
	while(*a) {
		if (!hashmap[*a]) {
			hashmap[*a] = 1;
			*(op+count) = *a;
			printf("stored %c at loc %d\n", op[count], count);
			count++;
		} else {
			if(count%10 == 0) {
				char *x = realloc(op, count*2*sizeof(char));
			}
		}
		a++;
	}
	op[count] = '\0';
	return op;
}

int main(int argc, char **argv)
{

	char a[256];
	printf("enter string: \n");
	scanf("%s", a);
	char *op = removedups(a);
	printf("without dups is %s\n", op);
	return 0;
}