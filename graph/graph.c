/*
    Simple graph in C, supported operations:
    add_vertex()
    add_edge()
    graph_initiate(vertex, vertex) # using DFS for now
    graph_bfs(vertex) # Using BFS - shortest path to all nodes from a given node
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include <stdbool.h>
#include "graph.h"

int main(int argc, char **argv)
{
    // keep track of head
    vertex *root = malloc(sizeof(*root));
    root->nextvertex = NULL;
    root = NULL;

    // keep track of last vertext
    vertex *last = malloc(sizeof(*last));
    last->nextvertex = NULL;

    last = root;

    graph_add('a', &root, &last);
    graph_add('b', &root, &last);
    graph_add('c', &root, &last);
    graph_add('d', &root, &last);
    graph_add('e', &root, &last);
    graph_add('f', &root, &last);
    graph_add('g', &root, &last);
    graph_add('h', &root, &last);

    graph_edge('a', 'b', &root, &last);
    graph_edge('a', 'c', &root, &last);
    graph_edge('b', 'c', &root, &last);
    graph_edge('a', 'd', &root, &last);
    graph_edge('a', 'e', &root, &last);
    graph_edge('c', 'd', &root, &last);
    graph_edge('d', 'e', &root, &last);
    graph_edge('d', 'f', &root, &last);
    graph_edge('d', 'g', &root, &last);
    graph_edge('c', 'h', &root, &last);

    printf("path between %c and %c exists? %d\n", 'a', 'e', graph_initiate('a', 'e', &root, DFS));
    printf("path between %c and %c exists? %d\n", 'd', 'e', graph_initiate('d', 'e', &root, DFS));
    printf("path between %c and %c exists? %d\n", 'c', 'e', graph_initiate('c', 'e', &root, DFS));
    printf("path between %c and %c exists? %d\n", 'b', 'e', graph_initiate('b', 'e', &root, DFS));
    printf("path between %c and %c exists? %d\n", 'b', 'a', graph_initiate('b', 'a', &root, DFS));
    printf("path between %c and %c exists? %d\n", 'f', 'a', graph_initiate('f', 'a', &root, DFS));
    printf("path between %c and %c exists? %d\n", 'b', 'f', graph_initiate('b', 'f', &root, DFS));
    printf("path between %c and %c exists? %d\n", 'd', 'f', graph_initiate('d', 'f', &root, DFS));
    printf("path between %c and %c exists? %d\n", 'd', 'g', graph_initiate('d', 'g', &root, DFS));
    printf("path between %c and %c exists? %d\n", 'c', 'h', graph_initiate('c', 'h', &root, DFS));

    printf("shortest path between all nodes:\n");
    printf("BFS from node %c exists? %d\n", 'b', graph_initiate('b', '\0', &root, BFS));
    printf("BFS from node %c exists? %d\n", 'a', graph_initiate('a', '\0', &root, BFS));
    printf("BFS from node %c exists? %d\n", 'x', graph_initiate('x', '\0', &root, BFS));
    printf("BFS from node %c exists? %d\n", 'c', graph_initiate('c', '\0', &root, BFS));

    return 0;
}

void graph_add(ge c, vertex **root, vertex **last)
{
    vertex *temp = *root;
    printf("root points to %p\n", *root);
    while (temp != NULL) {
        // vertex already exists in graph, ignore insert
        if (temp->element == c) {
            return;
        }
        temp = temp->nextvertex;
    }

    vertex *v = malloc(sizeof(*v));
    v->element = c;
    v->nextedge = NULL;
    // first vertex in graph
    if (*root == NULL) {
        *root = v;
        *last = *root;
    } else {
        (*last)->nextvertex = v;
        *last = v;
    }
}

void graph_edge(ge from, ge to, vertex **root, vertex **last)
{
    vertex *temp = *root;
    vertex *fromptr, *toptr;
    int count = 0;
    while (temp != NULL) {
        // ensure both the vertices are present before drawing an edge
        if (temp->element == from) {
            fromptr = temp;
            count++;
        }
        if (temp->element == to) {
            toptr = temp;
            count++;
        }

        temp = temp->nextvertex;
    }
    if (count < 2) {
        printf("either %c or %c vertices do not exist in graph\n", from, to);
        return;
    }

    edge *newedge = malloc(sizeof(*newedge));
    newedge->nextedge = NULL;
    newedge->link = toptr;

    if (fromptr->nextedge == NULL) {
        fromptr->nextedge = newedge;
        printf("added edge %c%c\n", fromptr->element, fromptr->nextedge->link->element);
        return;
    }
    edge *node = fromptr->nextedge;
    while (1) {
        printf("found edge %c%c\n", fromptr->element, (node)->link->element);
        if (node->nextedge == NULL) {
            break;
        } else {
            node = node->nextedge;
        }
    }
    node->nextedge = newedge;
    printf("added edge %c%c\n", node->link->element, node->nextedge->link->element);
}

// currently implemented in DFS
// this approach will not give if there are loops in a directed graph, primariy because we just use white and black (1 or 0)
// coloring scheme. if we introduce a white, black and gray scheme then it is possible to detect, because we will not be having
// any false positives
bool graph_check(ge start, ge end, vertex *parent)
{
    if (parent->element == end) {
        printf("found end %c\n", parent->element);
        return true;
    }
    if (parent->visited == 1) {
        printf("%c already visited, returning\n", parent->element);
        return false;
    }
    parent->visited = 1;
    edge *node = parent->nextedge;
    bool op = false;
    while (1) {
        if (node == NULL) {
            return false;
        }
        op = graph_check(node->link->element, end, node->link);
        printf("result of %c->%c check came %d\n", node->link->element, end, op);
        if (op == true) {
            return op;
        }
        node = node->nextedge;
    }
    return op;
}

bool graph_initiate(ge start, ge end, vertex **root, search_t STRATEGY)
{
    // first set all visited flags to 0
    // record where we need to start our search
    vertex *temp = (*root);
    vertex *begin = NULL;
    while (temp != NULL) {
        temp->visited = 0;
        if (temp->element == start) {
            begin = temp;
        }
        temp = temp->nextvertex;
    }
    if (begin == NULL) {
        printf("vertex %c does not exist in graph\n", start);
        return false;
    }
    printf("current search starts at %c\n", begin->element);
    if (STRATEGY == DFS) {
        return graph_check(start, end, begin);
    }
    if (STRATEGY == BFS) {
        queueroot = NULL;
        queuetail = NULL;
        return graph_bfs(begin);
    }
    return false;
}

// BFS from a given node, will give shortest path to all nodes from the given node
bool graph_bfs(vertex *root)
{
    if (root == NULL) {
        return false;
    }
    queue_add(root);
    root->visited = 1;
    while (queueroot != NULL) {
        vertex *tempnode = queue_pop();
        printf("%c->", tempnode->element);
        edge *tempedge = tempnode->nextedge;
        while (tempedge != NULL) {
            if (tempedge->link->visited != 1) {
                queue_add(tempedge->link);
                tempedge->link->visited = 1;
            }
            tempedge = tempedge->nextedge;
        }
    }
    printf("\n");
    return true;
}

void queue_add(vertex *current)
{
    queue *node = malloc(sizeof(*node));
    node->currentvertex = current;
    node->next = NULL;
    if (queueroot == NULL) {
        queueroot = node;
        queuetail = node;
    }
    else {
        queuetail->next = node;
        queuetail = node;
    }
}

vertex* queue_pop()
{
    if (queueroot == NULL) {
        return NULL;
    }
    queue *node = queueroot;
    queueroot = queueroot->next;

    if (queueroot == NULL) {
        queuetail = NULL;
    }

    return node->currentvertex;
}
