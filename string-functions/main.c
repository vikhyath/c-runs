#include <stdio.h>
#include <stdlib.h>

char* str_cpy(char*, const char*);
int str_cmp(const char *, const char *);
int str_len(const char *);
char* str_cat(char *, const char *);
char* str_chr(char *, char);
char* str_rev(char *);

int main(int argc, char **argv)
{
	char str1[] = "abcdewxyz";
	char str2[] = "";

	char s1[] = "vwxyz";
	str_cpy(str2, str1);

	printf("\nThe strings are %d", str_cmp(s1, str2));
	printf("\nThe length of the string str2 is %d\n", str_len(str2));

	//str_cat (s1, str2);

	printf("original string is %s\n",str1);
	char *rev = str_rev(str1);
	printf("reversed string is %s\n",str1);

	return 0;
}

char* str_chr (char *str, char c)
{
	while (*str) {
		if (*str == c) {
			return str;
		}
		else {
			str++;
		}
	}

	return NULL;
}

char* str_cat (char* dest, const char *source)
{
	for (;*dest;dest++);
	return (str_cpy (dest, source));
}

int str_len(const char *string)
{
	int i = 0;
	for(;*string; string++, i++);
	return i;
}

char* str_cpy(char *dest, const char *source)
{
	char *p = dest;
	while (*source != '\0') {
		*dest++ = *source++;
	}
	*dest = '\0';
	return p;
}

int str_cmp(const char *s1, const char *s2)
{
	int same = 1;
	while (*s1 != '\0' || *s2 != '\0') {
		if (*s1++ != *s2++) {
			same = 0;
			break;
		}
	}
	return same;
}

char* str_rev(char *s)
{
	//printf("length of string is: %d\n", len);
	int len, start;
	for (len = str_len(s) - 1, start = 0; start < len; start++, len--) {
		//printf("XORing %c with %c\n", *s, s[len]);
		s[start] ^= s[len];
		s[len] ^= s[start];
		s[start] ^= s[len];
	}
	return s;
}
