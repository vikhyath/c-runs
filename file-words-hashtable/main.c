#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define table 100

typedef struct node {
	struct node *next;
	char *data;
} hashtable;

unsigned long hashstring(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void createNode(hashtable **ptr, char *data)
{
	*ptr = malloc(sizeof(*ptr));
	(*ptr)->data = malloc(sizeof(char) * strlen(data));
	(*ptr)->data = data;
	(*ptr)->next = NULL;
}

void printList(hashtable *ptr)
{
	while(ptr != NULL) {
		printf("%s->", ptr->data);
		ptr = ptr->next;
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	hashtable *hash[table];

	FILE *f = fopen("test.txt", "r");
	if (f == NULL) {
		printf("cant find file\n");
	}
	char temp[100];
	while (fscanf(f, "%s", temp) == 1) {
		printf("%s\n", temp);
		unsigned long x = hashstring(temp);
		int bucket = (int) (x%table);
		if (hash[bucket] == NULL) {
			createNode(&hash[bucket], temp);
			printf("need to init bucket here\n");
		} else {
			hashtable *ptr = hash[bucket];
			while(ptr->next != NULL) {
				ptr = ptr->next;
			}
			hashtable *element;
			createNode(&element, temp);
			ptr->next = element;
		}
		printf("%lu\n", (x%table));
	}

	rewind(f);
	while (fscanf(f, "%s", temp) == 1) {
		unsigned long x = hashstring(temp);
		int bucket = (int) (x%table);
		printList(hash[bucket]);
	}

	return 0;
}
