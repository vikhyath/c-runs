#include <stdio.h>
#include <stdlib.h>

int valid(char c) {
	return (c >= '0' && c <= '9') ? 1 : 0;
}

int stringtoint(char *str)
{
	char *copy = str;
	int sign = 1;
	if (*str == '-') {
		sign = -1;
		str++;
	}

	int res = 0;
	while (*str) {
		if (!valid(*str)) {
			printf("invalid string %s\n", copy);
			exit(0);
		}
		// technically getting multiplied by 10, in other words res = res*10 + (*str - '0')
		res = (res << 3) + (res << 1) + (*str - '0');
		str++;
	}
	return res * sign;
}

int len(char *str)
{
	int i = 0;
	for(; *str != '\0'; str++, i++);
	return i;
}

int main(int argc, char **argv)
{
	char str[80];
	printf("enter your string...\n");
	scanf("%s", str);
	//printf("you have entered %s\n", str);
	if (len(str) != 0) {
		int res = stringtoint(str);
		printf("result is %d\n", res);
	}
	return 0;
}
