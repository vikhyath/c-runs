#include <stdio.h>
#include <stdlib.h>

void alterstring(char *str)
{
	int x = strlen(str);
	int i = 0, j = 0;
	for (i=0; i < x; i++) {
		if (str[i] == 'b') {
			str[j] = ' ';
			j++;
			printf("in case(if) b and storing space at %d\n", j);
			//ignore
		} else if (str[i] == 'c') {
			printf("in case(else if) c\n");
			if (j > 0 && str[j-1] == 'a') {
				j--;
				str[j] = ' ';
				printf("in case previous-a and storing space at %d\n", j);
				//ignore
			} else if (j > 0 && str[j-1] != 'a'){
				str[j] = str[i];
				printf("in case not previous and storing %c at %d\n", str[j], j);
				j++;
			}
		} else {
			printf("in case(else) %c\n", str[j]);
			str[j] = str[i];
			printf("stored %c at %d position\n", str[j], j);
			j++;
		}
	}
	str[j] = '\0';
}

int main(int argc, char **argv)
{
	char str[10];
	printf("enter string\n");
	scanf("%s", str);

	alterstring(str);

	printf("altered string is %s\n", str);
	return 0;
}
