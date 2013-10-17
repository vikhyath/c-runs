#include <stdio.h>
#include <stdlib.h>

typedef struct dl {
	struct dl *prev;
	struct dl *next;
	int data;
}dlist;

int addList(dlist **head, dlist **tail, int data)
{
	dlist *node = malloc(sizeof(*node));
	if (node == NULL) {
		return 0;
	}
	if (*head == NULL) {
		node->prev = NULL;
		node->next = NULL;
		node->data = data;
		*head = node, *tail = node;
	} else {
		(*tail)->next = node;
		node->prev = *tail;
		node->next = NULL;
		node->data = data;
		*tail = node;
	}

	return 1;
}

void printList(dlist *temp)
{
	if (temp == NULL) {
		printf("Empty list, nothing to print\n");
	}
	while(temp) {
		printf("%d\n", temp->data);
		temp = temp->next;
	}
}

int deleteNode(dlist **head, dlist **tail, int data)
{
	dlist *temp = *head;
	if (*head == NULL && *tail == NULL) {
		return -1;
	}
	while (temp) {
		if (temp->data == data) {
			if (*head == *tail) {
				printf("case 1-element for %d\n", temp->data);
				*head = NULL;
				*tail = NULL;
			} else if (temp->next != NULL && temp->prev != NULL) {
				printf("case middle for %d\n", temp->data);
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
			} else if (temp->next == NULL) {
				printf("case last for %d\n", temp->data);
				temp->prev->next = NULL;
				*tail = temp->prev;
			} else if (temp->prev == NULL) {
				printf("case first for %d\n", temp->data);
				*head = temp->next;
				temp->next->prev = NULL;
			}
			temp->next = NULL;
			temp->prev = NULL;
			free(temp);
			return 1;
		}
		temp = temp->next;
	}

	return 0;
}

void evaluate(int ret)
{
	switch(ret) {
		case 0:
			printf("node not found\n");
			break;
		case 1:
			printf("node deleted\n");
			break;
		case -1:
			printf("list empty");
			break;
	}
}

int main(int argc, char **argv)
{
	dlist *head = malloc(sizeof(*head));
	dlist *tail = malloc(sizeof(*head));
	head = NULL, tail = NULL;

	int ret, i = 0;
	for(i = 0; i < 5; i++) {
		ret = addList(&head, &tail, i);
		if (ret == 0) {
			printf("malloc problems");
			exit(0);
		}
	}
	printList(head);
	ret = deleteNode(&head, &tail, 4);
	evaluate(ret);
	printList(head);

	ret = deleteNode(&head, &tail, 0);
	evaluate(ret);
	printList(head);

	ret = deleteNode(&head, &tail, 2);
	evaluate(ret);
	printList(head);

	ret = deleteNode(&head, &tail, 1);
	evaluate(ret);
	printList(head);

	ret = deleteNode(&head, &tail, 3);
	evaluate(ret);
	printList(head);

	return 0;
}
