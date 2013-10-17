#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	struct node *next;
	int data;
}queue;

int queue_push(queue **first, queue **last, int data)
{
	queue *node = malloc(sizeof(*node));

	node->data = data;
	node->next = NULL;

	if (*first == NULL) {
		*first = node;
		*last = *first;
	} else {
		(*last)->next = node;
		*last = node;
	}

	return 0;
}

int queue_pop(queue **first)
{
	if (*first == NULL) {
		return -1;
	}

	queue *temp = *first;
	int retdata = temp->data;

	*first = (*first)->next;

	free(temp);
	return retdata;
}

int main(int argc, char **argv)
{
	queue *first = malloc(sizeof(*first));
	queue *last = malloc(sizeof(*last));
	first = NULL;
	last = NULL;

	int x = queue_push(&first, &last, 5);
	x = queue_push(&first, &last, 10);
	x = queue_push(&first, &last, 15);
	x = queue_push(&first, &last, 25);
	x = queue_push(&first, &last, 35);

	while(first) {
		x = queue_pop(&first);
		printf("popped %d\n", x);
	}

	return 0;
}