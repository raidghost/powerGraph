#ifndef CHECK
#define CHECK

#include "structs.h"

unsigned int nbEdges(GRAPH *g);
int commonDegree(GRAPH *g);
GRAPH_LIST* checkEkCert(GRAPH_LIST* powerGraph, EK_CERT* ekCert, int field);

#endif
