#include <stdio.h>
#include <stdlib.h>

typedef struct list {
	struct list *next;
	int data;
}stack;

int stack_push(stack **head, int data)
{
	stack *node = malloc(sizeof(*node));

	node->data = data;
	node->next = *head;
	*head = node;
}

int stack_pop(stack **head)
{
	if (*head == NULL) {
		return -1;
	}

	stack *temp = *head;
	int retdata = temp->data;
	*head = (*head)->next;
	free(temp);

	return retdata;
}

int peek(stack **head)
{
	if (*head == NULL) {
		return -1;
	}

	return (*head)->data;
}

void stack_sort(stack **head1, stack **head2)
{
	if (*head1 == NULL) {
		return;
	}

	while (*head1) {
		int x = stack_pop(&*head1);
		while (*head2 != NULL && x > peek(&*head2)) {
				int pop = stack_pop(&*head2);
				stack_push(&*head1, pop);
		}
		stack_push(&*head2, x);
	}
}

void stack_print(stack *head)
{
	while(head) {
		printf("%d\n", head->data);
		head = head->next;
	}
}

int main(int argc, char **argv)
{
	stack *head1 = malloc(sizeof(*head1));
	stack *head2 = malloc(sizeof(*head2));

	head1 = NULL;
	head2 = NULL;

	stack_push(&head1, 12);
	stack_push(&head1, 5);
	stack_push(&head1, 7);
	stack_push(&head1, 15);
	stack_push(&head1, 0);
	stack_push(&head1, -1);
	stack_push(&head1, 23);

	stack_sort(&head1, &head2);

	stack_print(head2);

}