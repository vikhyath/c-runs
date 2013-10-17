#include <stdio.h>

int main(void) {
	char a[] = {'1','2','3','4','5','\0'};

	printf("%c\n", 2[a]);

	/*
	char strA[80] = "A string to be used for demonstration purposes";
	char strB[180] = "012345678901234567890123456789012345678901234567890123456789";

	char *pA;
	char *pB;

	puts(strA);
	pA = strA;
	puts(pA);
	pB = strB;
	putchar('\n');

	while(*pA!='\0') {
		*pB++=*pA++;
	}
	*pB = '\0';
	puts(strB);
	printf("below this\n");
	puts(strB);*/
	return 0;
}
