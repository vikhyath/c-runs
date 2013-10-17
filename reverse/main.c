#include <stdio.h>
#include <string.h>

void str_rev(char *str)
{
	char *ptr = str;
	char temp;
	while (*ptr) {
		ptr++;
	}
	ptr--;
	while(str < ptr) {
		temp = *str;
		*str++ = *ptr;
		*ptr-- = temp;
		//str++;
		//ptr--;
	}
}

str_rev_xor(char *str)
{
	int len = strlen(str);
	int i = 0, j = len-1;
	while (i < j) {
		str[i] ^= str[j];
		str[j] ^= str[i];
		str[i] ^= str[j];
		i++, j--;
	}
}

int main(int argc, char **argv)
{
	char str[100];
	printf("enter string....");
	scanf("%s", str);

	str_rev_xor(str);
	printf("reversed string is %s\n", str);
	return 0;
}