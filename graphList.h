#ifndef GRAPHLIST
#define GRAPHLIST

GRAPH_LIST* addVertex(GRAPH_LIST *g, NUPLE *v);
GRAPH_LIST* searchVertex(GRAPH_LIST *g, NUPLE v);
void addEdge(GRAPH_LIST* g, NUPLE u, NUPLE v);
void freeGraphList(GRAPH_LIST *g);

#endif
