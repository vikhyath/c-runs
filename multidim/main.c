#include <stdio.h>
#include <stdlib.h>

char multi[5][10];


int main (int argc, char **argv)
{
	/*typedef int Array[10];
	Array arr[5];
	int **p;

	p = &arr[0];

	printf("size of array is %lu\n",sizeof(*p));*/

	char s[][];
	s[0] = "aa";
	s[1] = "bb";
	s[2] = "cc";
	s[3] = "dd";


	char **arg;
	arg = s;
	int i = 0;
	printf("asdsda\n");
	/*for (i=0; i<4; i++) {
		*(arg)+i = (char *)malloc(6*sizeof(char));
	}
	arg[3] = "hey";*/
	printf("%lu\n",sizeof(arg));
	return 0;
}
