#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct triestruct {
    int val;
    struct triestruct *next;
    struct triestruct *child;
    char c;
} trie;

void trie_add(trie **root, char name[], int val)
{
    printf("got call\n");
    int i = 0;
    trie *parent = *root;
    printf("root is %p and name size is %lu\n", *root, strlen(name));
    // iterate and create a new node for every character
    for (; i < strlen(name); i++) {
        int found = 0;
        // if child exists and is not end of line
        if (parent->child != NULL && parent->child->c != '\0') {
            // check each child to see if the name[i] matches
            trie *node = parent->child;
            while (node != NULL) {
                // found match
                if (node->c == name[i]) {   
                    found = 1;
                    parent = node;
                    break;
                } else {
                    // traverse to next child
                    parent = node;
                    node = node->next;
                }
            }
        }
        // if found is still 0, create new node and set appropriate values
        if (0 == found) {
            trie *newnode = malloc(sizeof(*newnode));
            newnode->next = NULL;
            newnode->child = NULL;
            newnode->c = name[i];

            // set as parents child else set as parents childs next
            if (parent->child == NULL) {
                parent->child = newnode;
                printf("created node with parent (no child) %p for char %c at %p\n", parent, newnode->c, newnode);
                parent = parent->child;
            } 
            else if (parent != NULL) {
                parent->next = newnode;
                printf("created node with parent (next node) %p for char %c at %p\n", parent, newnode->c, newnode);
                parent = newnode;
            }
        }
    }

    // if current parent has no val attached, attach one
    // overwrite childs val if parent has a child
    if (parent->child != NULL) {
        parent->child->val = val;
        printf("updated value of node at %p with %d\n", parent->child, val);
    } else {
        trie *newnode = malloc(sizeof(*newnode));
        newnode->next = NULL;
        newnode->child = NULL;
        newnode->val = val;
        newnode->c = '\0'; // end of line
        printf("created node with parent %p and attached value %d at %p\n", parent, newnode->val, newnode);
        parent->child = newnode;
    }

}

int trie_search(trie **root, char name[])
{
    trie *parent = *root;
    int i = 0;
    for (; i < strlen(name); i++) {
        if (parent->child != NULL) {
            trie *node = parent->child;
            while (node != NULL) {
                if (name[i] == node->c) {
                    parent = node;
                    break;
                } else {
                    node = node->next;
                }
            }
        } else {
            return -1;
        }
    }
    if (parent->child != NULL) {
        return parent->child->val;
    } else {
        return -1;
    }
}

int main(int argc, char **argv)
{
    trie *root = malloc(sizeof(*root));
    root->next = NULL;
    root->child = NULL;
    printf("root init\n");

    trie_add(&root, "amy", 56);
    trie_add(&root, "ann", 15);
    trie_add(&root, "emma", 30);
    trie_add(&root, "rob", 27);
    trie_add(&root, "rob", 28);
    trie_add(&root, "roger", 52);
    trie_add(&root, "roger", 55);
    trie_add(&root, "ann", 20);

    printf("value of amy is %d\n", trie_search(&root, "amy"));
    printf("value of ann is %d\n", trie_search(&root, "ann"));
    printf("value of emma is %d\n", trie_search(&root, "emma"));
    printf("value of rob is %d\n", trie_search(&root, "rob"));
    printf("value of roger is %d\n", trie_search(&root, "roger"));
    printf("value of robb is %d\n", trie_search(&root, "robb"));
    printf("value of griggs is %d\n", trie_search(&root, "griggs"));
    return 0;
}