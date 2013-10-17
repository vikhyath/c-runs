#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	struct node *next;
	int data;
}stack;

int stack_push(stack **head, int data)
{
	stack *node = malloc(sizeof(*node));
	if (node == NULL) {
		return -1;
	}

	node->data = data;
	node->next = *head;
	*head = node;

	return 0;
}

int stack_pop(stack **head)
{
	if (*head == NULL) {
		return -1;
	}

	stack *temp = *head;
	int retdata = (*head)->data;
	*head = (*head)->next;

	temp->next = NULL;
	free(temp);

	return retdata;
}

int main(int argc, char **argv)
{
	stack *head = malloc(sizeof(*head));
	head = NULL;

	int x = stack_push(&head, 12);
	x = stack_push(&head, 4);
	stack_push(&head, 6);

	while(head) {
		int data = stack_pop(&head);
		printf("popped %d\n", data);
	}
}