#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tag{                     /* the structure type */
    char lname[20];             /* last name */
    char fname[20];             /* first name */
    int age;                    /* age */
    float rate;                 /* e.g. 12.75 per hour */
}*p;

typedef int t;

int main (int argc, char **argv)
{
	printf ("hey");
	struct tag *p = malloc(sizeof(*p)) ;

	strcpy (p->lname, "jsons");

	printf ("%s\n", p->lname);

	t x = 10;

	printf("%d\n", x);
	return 0;
}
