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
    // printf("got call\n");
    int i = 0;
    trie *parent = *root;
    // printf("root is %p and name size is %lu\n", *root, strlen(name));
    // iterate and create a new node for every character
    // to save on string traversal twice, we can do a while (*name != NULL)
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
                // printf("created node with parent (no child) %p for char %c at %p\n", parent, newnode->c, newnode);
                parent = parent->child;
            } 
            else if (parent != NULL) {
                parent->next = newnode;
                // printf("created node with parent (next node) %p for char %c at %p\n", parent, newnode->c, newnode);
                parent = newnode;
            }
        }
    }

    // if current parent has no val attached, attach one
    // overwrite childs val if parent has a child
    if (parent->child != NULL) {
        parent->child->val = val;
        // printf("updated value of node at %p with %d\n", parent->child, val);
    } else {
        trie *newnode = malloc(sizeof(*newnode));
        newnode->next = NULL;
        newnode->child = NULL;
        newnode->val = val;
        newnode->c = '\0'; // end of line
        // printf("created node with parent %p and attached value %d at %p\n", parent, newnode->val, newnode);
        parent->child = newnode;
    }

}

int trie_search(trie **root, char name[])
{
    trie *parent = *root;
    int i = 0, found = 0;
    for (; i < strlen(name); i++) {
        // if child exists and child is not value
        // eg: trie has emma and search is for emm
        if (parent->child != NULL) {
            trie *node = parent->child;
            while (node != NULL) {
                // reset for each loop, we only care about final state of found
                found = 0;
                if (name[i] == node->c) {
                    parent = node;
                    found = 1;
                    break;
                } else {
                    found = 0;
                    node = node->next;
                }
            }
            // if node is null, we have not found any matching in the current search
            if (node == NULL) {
                return -1;
            }
        }
        // i/p string is too long, trie has rob and search is for robb
        else {
            return -1;
        }
    }
    // all letters matched in trie and there is nothing else left to search in children
    // that is, we are making sure we do not match substrings
    if (found != 0 && parent->child->c == '\0') {
        return parent->child->val;
    } else {
        return -1;
    }
}

trie* trie_delete(trie *root, trie *parent, char name[], int index)
{
    if (index < strlen(name)-1) {
        parent = root;
        trie *current = (root)->child;
        //printf("parent is %p and current is %p, checking for %c (currnet) to %c (input)\n", parent, current, current->c, name[index]);
        // current node matches char
        if (current->c == name[index]) {
            // happy
            //printf("found happy match %c\n", current->c);
        } else {
            //printf("checking for current->c: %c\n", current->c);
            // find the right node to del
            while (current->next != NULL) {
                parent = current;
                current = current->next;
                if (current->c == name[index]) {
                    //printf("found %c, breaking\n", current->c);
                    break;
                }
            }
        }
        root = trie_delete(current, parent, name, index+1);
        //printf("got back %p\n", root);
    } else {
        // delete the child, copy next to root if needed
        trie *child = root->child;
        if (child->next != NULL) {
            root->child = child->next;
        } else {
            root->child = NULL;
        }
        free(child);
    }
    // at the correct node, just delete it if the letter matches,
    // if it does not match it means we are at the correct level only, maybe we have deleted next?
    // or if child exists, then dont delete
    if ((root)->c != name[index] || (root)->child != NULL) {
        return root;
    }

    // if parent's child is same as the one being targeted for deletion,
    // i.e., we are making sure we copy parents next->next data into parents->next
    if ((parent)->child == root) {
        // attach next if needed
        //printf("current value of root is %c and parent is %c\n", (root)->c, (parent)->c);
        if ((root)->next != NULL) {
            //printf("attaching %c to %c\n", (root)->next->c, (parent)->c);
            (parent)->child = (root)->next;
        } else {
            (parent)->child = NULL;
        }
        free(root);
    } else {
        //printf("next guy case!!\n");
        // next guy
        // if  deleted node any next pointers, reassign them to parent
        trie *temp = (parent)->next;
        (parent)->next = temp->next;
        free(temp);
    }

    return parent;
}

void trie_delete_wrap(trie *root, trie *parent, char name[], int index)
{
    if (trie_search(&root, name) != -1) {
        trie_delete(root, parent, name, index);
        printf("deleted %s\n", name);
    } else {
        printf("%s does not exist in trie to delete\n", name);
    }
}

int main(int argc, char **argv)
{
    trie *root = malloc(sizeof(*root));
    root->next = NULL;
    root->child = NULL;
    printf("trie init\n");

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
    printf("value of emm is %d\n", trie_search(&root, "emm"));

    trie_delete_wrap(root, root, "amy", 0);
    printf("value of amy is %d\n", trie_search(&root, "amy"));
    printf("value of ann is %d\n", trie_search(&root, "ann"));
    trie_delete_wrap(root, root, "emma", 0);
    printf("value of emma is %d\n", trie_search(&root, "emma"));
    printf("value of rob is %d\n", trie_search(&root, "rob"));
    printf("value of roger is %d\n", trie_search(&root, "roger"));
    trie_delete_wrap(root, root, "emm", 0);
    trie_delete_wrap(root, root, "rob", 0);
    printf("value of rob is %d\n", trie_search(&root, "rob"));
    printf("value of roger is %d\n", trie_search(&root, "roger"));
    trie_delete_wrap(root, root, "ann", 0);
    printf("value of ann is %d\n", trie_search(&root, "ann"));
    printf("value of roger is %d\n", trie_search(&root, "roger"));
    trie_delete_wrap(root, root, "roger", 0);
    printf("value of roger is %d\n", trie_search(&root, "roger"));
    trie_delete_wrap(root, root, "roger", 0);

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
    printf("value of emm is %d\n", trie_search(&root, "emm"));


    return 0;
}