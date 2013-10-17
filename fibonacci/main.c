#include <stdio.h>

int fibrecurse(int n)
{
	if (n == 0) {
		return 0;
	}
	else if (n == 1) {
		return 1;
	}
	else {
		int x = (fibrecurse(n-1) + fibrecurse(n-2));
		return x;
	}
}

int main(int argc, char **argv)
{
	int c, n = 12, i=0;
	/*for ( c = 1 ; c <= n ; c++ )
	   {
	      printf("%d\n", fibrecurse(i));
	      i++;
	   }*/

	int c1 = 0, c2 = 1, x = 0;
	while (i <= n) {
		if (i <= 1)
			x = i;
		else {
			x = c1+c2;
			c1 = c2;
			c2 = x;
		}
		printf("%d\n", x);
		i++;
	}
	return 0;
}
