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

// must include function definitions here too
void graph_add(ge, vertex**, vertex**);
void graph_edge(ge, ge, vertex**, vertex**);
bool graph_check(ge, ge, vertex*);
bool graph_checkpath(ge, ge, vertex**);

#endif