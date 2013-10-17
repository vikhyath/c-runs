#include <stdio.h>
#include <stdlib.h>

typedef struct dl {
	struct dl *left;
	struct dl *right;
	struct dl *parent;
	int data;
} dlist;

int insert(dlist*, dlist*);

int addToBst(dlist **root, int data)
{
	dlist *node = malloc(sizeof(*node));
	if (node == NULL) {
		return 0;
	}
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;

	dlist *temp = *root;
	if (*root == NULL) {
		*root = node;
		//printf("first element %d\n", node->data);
		temp = *root;
	} else {
		int ret = insert(*root, node);
		if (ret == -1) {
			//printf("duplicate insert %d\n", node->data);
		}
	}

	return 1;
}

int insert(dlist *current, dlist *node)
{
	if (current->data == node->data) {
		return -1;
	}
	else if (node->data > current->data) {
		if (current->right == NULL) {
			current->right = node;
			node->parent = current;
			printf("%d is right of %d\n", node->data, current->data);
			return 1;
		} else {
			return(insert(current->right, node));
		}
	} else {
		if (current->left == NULL) {
			current->left = node;
			node->parent = current;
			printf("%d is left of %d\n", node->data, current->data);
			return 1;
		} else {
			return(insert(current->left, node));
		}
	}
}

void printList(dlist *current)
{
	if (current == NULL) {
		printf("Empty list, nothing to print\n");
	}
	if (current->left != NULL) {
		printList(current->left);
	}
	printf("%d\n", current->data);
	if (current->right != NULL) {
		printList(current->right);
	}
}

dlist* findNode(dlist *current, int data)
{
	if (current->data == data) {
		//printf("found %d\n", data);
		return current;
	}
	if (current->data < data) {
		if (current->right != NULL) {
			//printf("going right %d to %d\n", current->data, current->right->data);
			findNode(current->right, data);
		} else {
			return NULL;
		}
	} else {
		if (current->left != NULL) {
			//printf("going left %d to %d\n", current->data, current->left->data);
			findNode(current->left, data);
		} else {
			return NULL;
		}
	}
}

int deleteNode(dlist **root, int data)
{
	dlist *node  = findNode(*root, data);
	if (node == NULL) {
		printf("cannot find %d\n", data);
	} else {
		if (node->right == NULL && node->left == NULL) {
			if (node->parent != NULL) {
				if (node->data < node->parent->data) {
					node->parent->left = NULL;
				} else {
					node->parent->right = NULL;
				}
				node->parent = NULL;
			} else { //root node
				*root = NULL;
			}
		} else {
			//printf("trying to delete %d\n", node->data);
			if (node->left != NULL && node->right != NULL) {
				if (node->parent != NULL) {
					node->left->parent = node->parent;
					if (node->left->data < node->parent->data) {
						node->parent->left = node->left;
					} else {
						node->parent->right = node->left;
					}
				} else {
					*root = node->left;
					node->left->parent = NULL;
				}
				node->right->parent = NULL;
				printf("trying to merge left: %d and right: %d\n", node->left->data, node->right->data);
				int ret = insert(node->left, node->right);
				if (ret == -1) {
					printf("some serious error!\n");
					exit(0);
				}
			} else if (node->right == NULL) {
				if (node->parent != NULL) {
					node->left->parent = node->parent;
					if (node->left->data < node->parent->data) {
						node->parent->left = node->left;
					} else {
						node->parent->right = node->left;
					}
				} else {
					*root = node->left;
				}
			} else if (node->left == NULL) {
				if (node->parent != NULL) {
					node->right->parent = node->parent;
					if (node->right->data < node->parent->data) {
						node->parent->left = node->right;
					} else {
						node->parent->right = node->right;
					}
				} else {
					*root = node;
				}
			}
		}
		node->left = NULL;
		node->right = NULL;
		node->parent = NULL;
		printf("deleted %d\n", node->data);
		free(node);
	}
	return 0;
}

int main(int argc, char **argv)
{
	dlist *root = malloc(sizeof(*root));
	root = NULL;
	int ret, i = 0;
	for(i = 0; i < 10; i=i+2) {
		ret = addToBst(&root, i);
		if (ret == 0) {
			printf("malloc problems");
			exit(0);
		}
	}
	for(i = -6; i < 10; i=i+2) {
		ret = addToBst(&root, i);
		if (ret == 0) {
			printf("malloc problems");
			exit(0);
		}
	}
	addToBst(&root, 1);
	addToBst(&root, -5);
	addToBst(&root, -8);
	addToBst(&root, -7);
	addToBst(&root, -9);
	addToBst(&root, 5);
	addToBst(&root, 3);
	addToBst(&root, 7);
	printList(root);
	printf("\n-------\n");
	deleteNode(&root, 0);
	deleteNode(&root, 2);
	deleteNode(&root, 4);
	printList(root);
	return 0;
}
