#include <stdio.h>
#include <stdlib.h>

#define limit 3
#define stackcount 0

typedef struct list {
	struct list *next;
	int data;
}stack;

int stack_push(stack **stacks, int data, int *totalelements, int *stacksactive)
{
	int stackseq = *totalelements/limit;
	stack *node = malloc(sizeof(*node));

	printf("here for stackseq %d and val is %p\n", stackseq, stacks[stackseq]);
	if (stacks[stackseq] == NULL) {
		printf("here again for %d\n", stackseq);
		(*stacksactive)++;
	}
	
	node->data = data;
	node->next = stacks[stackseq];
	stacks[stackseq] = node;

	(*totalelements)++;

	return 0;

}

int stack_print(stack *head)
{
	while(head) {
		printf("%d\n", head->data);
		head = head->next;
	}
}

int stack_pop(stack **stacks, int *totalelements, int *stacksactive)
{
	if (*stacksactive == 0) {
		return -1;
	}
	stack *temp = stacks[*stacksactive-1];
	stacks[*stacksactive-1] = temp->next;
	int retdata = temp->data;

	if (stacks[*stacksactive-1] == NULL) {
		(*stacksactive)--;
	}
	(*totalelements)--;
	free(temp);
	printf("popped %d\n", retdata);
	return retdata;
}

int main(int argc, char **argv)
{
	stack *stacks[100] = {NULL};
	int i = 0;

	for (i = 0;i < 100; i++) {
		stacks[i] = NULL;
	}

	stack **stack_ptr = &stacks[0];
	printf("stackseq[1] address is %p\n", &stacks[1]);
	int totalelements = 0, stacksactive = 0;

	stack_push(stack_ptr, 3, &totalelements, &stacksactive);
	stack_push(stack_ptr, 4, &totalelements, &stacksactive);
	stack_push(stack_ptr, 5, &totalelements, &stacksactive);

	stack_push(stack_ptr, 6, &totalelements, &stacksactive);
	stack_push(stack_ptr, 7, &totalelements, &stacksactive);
	stack_push(stack_ptr, 8, &totalelements, &stacksactive);

	stack_print(stacks[0]);
	stack_print(stacks[1]);

	printf("total active stacks are: %d\n", stacksactive);

	stack_pop(stack_ptr, &totalelements, &stacksactive);
	stack_pop(stack_ptr, &totalelements, &stacksactive);
	stack_pop(stack_ptr, &totalelements, &stacksactive);
	stack_pop(stack_ptr, &totalelements, &stacksactive);

	printf("total active stacks are: %d\n", stacksactive);
	stack_push(stack_ptr, 18, &totalelements, &stacksactive);
	stack_pop(stack_ptr, &totalelements, &stacksactive);
	stack_pop(stack_ptr, &totalelements, &stacksactive);
	stack_pop(stack_ptr, &totalelements, &stacksactive);
	stack_pop(stack_ptr, &totalelements, &stacksactive);
	
	printf("total active stacks are: %d\n", stacksactive);

	stack_push(stack_ptr, 18, &totalelements, &stacksactive);

	stack_pop(stack_ptr, &totalelements, &stacksactive);

	printf("total active stacks are: %d\n", stacksactive);
}