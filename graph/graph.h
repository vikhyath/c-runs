#ifndef graph_header
#define graph_header

typedef char ge; // graph element typedef

typedef struct gedge {
    struct gedge *nextedge;
    struct gvertex *link;
}edge;

typedef struct gvertex {
    ge element;
    struct gvertex *nextvertex;
    struct gedge *nextedge;
    int visited;
}vertex;

typedef struct gqueue {
    struct gvertex *currentvertex;
    struct gqueue *next;
}queue;

typedef enum {
    DFS,
    BFS
} search_t;

queue *queueroot;
queue *queuetail;

// must include function definitions here too
void graph_add(ge, vertex**, vertex**);
void graph_edge(ge, ge, vertex**, vertex**);
bool graph_check(ge, ge, vertex*);
bool graph_initiate(ge, ge, vertex**, search_t);
bool graph_bfs(vertex*);
void queue_add(vertex*);
vertex* queue_pop();

#endif