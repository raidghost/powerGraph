#ifndef POWERGRAPH
#define POWERGRAPH

#include "structs.h"

GRAPH_LIST* genPowerGraph(GRAPH* g, unsigned int p, int supportMax);
EK_CERT findEkCert(GRAPH* g, GRAPH_LIST* powerGraph, unsigned int p, int field);
void freeEkCert(EK_CERT *ekCert);

#endif
