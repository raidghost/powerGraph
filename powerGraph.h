#ifndef POWERGRAPH
#define POWERGRAPH

#include "structs.h"

GRAPH_LIST* genPowerGraph(GRAPH* g, unsigned int p, int supportMax);
EK_CERT ekCert(GRAPH* g, GRAPH_LIST* powerGraph, unsigned int p, int field);

#endif
