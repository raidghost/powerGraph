#ifndef GRAPHLIST
#define GRAPHLIST

#include "structs.h"

GRAPH_LIST* addVertex(GRAPH_LIST *g, NUPLE *v);
unsigned long getNbVertices(GRAPH_LIST* g);
GRAPH_LIST* searchVertex(GRAPH_LIST *g, NUPLE v);
GRAPH_LIST* getVertexByIndex(GRAPH_LIST *g, unsigned int index);
int getIndexByVertex(GRAPH_LIST* g, GRAPH_LIST* u);
void addEdge(GRAPH_LIST* g, NUPLE u, NUPLE v);
void addEdgeFast(GRAPH_LIST* u, GRAPH_LIST* v);
void freeGraphList(GRAPH_LIST *g);
GRAPH graphList2Mat(GRAPH_LIST* graphList);

#endif
