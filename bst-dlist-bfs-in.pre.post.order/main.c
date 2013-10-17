#include <stdio.h>
#include <stdlib.h>

typedef struct list {
	struct list *left;
	struct list *right;
	struct list *parent;
	int data;
}bst;

typedef struct q {
	struct q *next;
	bst *element;
}queue;

bst *find_spot(bst **root, int data) {
	bst *temp = *root;
	while(1) {
		if (data < temp->data) {
			if (temp->left == NULL) {
				return temp;
			}
			temp=temp->left;
		} else {
			if (temp->right == NULL) {
				return temp;
			}
			temp = temp->right;
		}
	}
}

int bst_add(bst **root, int data)
{
	bst *node = malloc(sizeof(*node));
	if (node == NULL) {
		return -1; //some error
	}
	node->data = data;
	node->left = NULL;
	node->right = NULL;

	if (*root == NULL) {
		*root = node;
		node->parent = NULL;
	} else {
		bst *parent = find_spot(&*root, data);
		if (data < parent->data) {
			parent->left = node;
		} else {
			parent->right = node;
		}
		node->parent = parent;
	}
	return 0;
}

int bst_inorder_print(bst *parent)
{
	if(parent != NULL) {
		bst_inorder_print(parent->left);
		printf("%d ", parent->data);
		bst_inorder_print(parent->right);
	}
}

int bst_preorder_print(bst *parent)
{
	if(parent != NULL) {
		printf("%d ", parent->data);
		bst_preorder_print(parent->left);
		bst_preorder_print(parent->right);
	}
}

int bst_postorder_print(bst *parent)
{
	if(parent != NULL) {
		bst_postorder_print(parent->left);
		bst_postorder_print(parent->right);
		printf("%d ", parent->data);

	}
}

void enqueue(queue **first, queue **last, bst **element)
{
	queue *node = malloc(sizeof(*node));
	node->element = *element;
	node->next = NULL;
	if (*first == NULL && *last == NULL) {
		*first = node;
		*last = node;
	} else {
		(*last)->next = node;
		*last = node;
	}
}

bst* dequeue(queue **first, queue **last)
{
	queue *temp = *first;
	bst *retnode = temp->element;
	(*first) = (*first)->next;
	if (*first == NULL) {
		*last = NULL;
	}
	free(temp);
	return retnode;
}

void bst_bfs_search(bst **root, queue **first, queue **last) {
	enqueue(&*first, &*last, &*root);
	while(*first != NULL && *last != NULL) {
		bst *retnode = dequeue(&*first, &*last);
		printf("%d ", retnode->data);
		if (retnode->left != NULL) {
			enqueue(&*first, &*last, &(retnode->left));
		}
		if (retnode->right != NULL) {
			enqueue(&*first, &*last, &(retnode->right));
		}
	}
	printf("\n");
}

int bst_check_balance(bst *root)
{
	if (root == NULL) {
		return 0;
	}
	int left = bst_check_balance(root->left);
	int right = bst_check_balance(root->right);
	if(left == -1 || right == -1) {
		return -1;
	}
	if (left != right) {
		return -1;
	} else {
		return left+1;
	}
}

int main(int argc, char **argv)
{
	bst *root = malloc(sizeof(*root));
	queue *first = malloc(sizeof(*first));
	queue *last = malloc(sizeof(*last));
	first = NULL;
	last = NULL;
	root = NULL;
	int ret = bst_add(&root, 50);
	ret = bst_add(&root, 40);
	ret = bst_add(&root, 45); 
	ret = bst_add(&root, 100);
	ret = bst_add(&root, 70);
	ret = bst_add(&root, 150);
	ret = bst_add(&root, 30);
	//ret = bst_add(&root, 20);
	//ret = bst_add(&root, 25);

	bst_inorder_print(root);
	printf("\n");
	bst_preorder_print(root);
	printf("\n");
	bst_postorder_print(root);
	printf("\n");

	bst_bfs_search(&root, &first, &last);
	int balance = bst_check_balance(root);
	if (balance == -1) {
		printf("the tree is not balanced\n");
	} else {
		printf("the tree is balanced\n");
	}
}
