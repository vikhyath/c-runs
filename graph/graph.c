/*
    Simple graph in C, supported operations:
    add_vertex()
    add_edge()
    graph_checkpath(vertex, vertex) # using DFS for now
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

    graph_edge('a', 'b', &root, &last);
    graph_edge('a', 'c', &root, &last);
    graph_edge('b', 'c', &root, &last);
    graph_edge('a', 'd', &root, &last);
    graph_edge('a', 'e', &root, &last);
    graph_edge('c', 'd', &root, &last);
    graph_edge('d', 'e', &root, &last);

    printf("path between %c and %c exists? %d\n", 'a', 'e', graph_checkpath('a', 'e', &root));
    printf("path between %c and %c exists? %d\n", 'd', 'e', graph_checkpath('d', 'e', &root));
    printf("path between %c and %c exists? %d\n", 'c', 'e', graph_checkpath('c', 'e', &root));
    printf("path between %c and %c exists? %d\n", 'b', 'e', graph_checkpath('b', 'e', &root));
    printf("path between %c and %c exists? %d\n", 'b', 'a', graph_checkpath('b', 'a', &root));
    printf("path between %c and %c exists? %d\n", 'f', 'a', graph_checkpath('f', 'a', &root));
    printf("path between %c and %c exists? %d\n", 'b', 'f', graph_checkpath('b', 'f', &root));

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

bool graph_checkpath(ge start, ge end, vertex **root)
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
    return graph_check(start, end, begin);
}