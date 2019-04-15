#ifndef POWERGRAPH
#define POWERGRAPH

#include "structs.h"

GRAPH_LIST* genPowerGraph(GRAPH* g, unsigned int p, int supportMax);
EK_CERT_F2 findEkCertF2(GRAPH* g, GRAPH_LIST* powerGraph, unsigned int p);
EK_CERT_R findEkCertR(GRAPH* g, GRAPH_LIST* powerGraph, unsigned int p);
void freeEkCertF2(EK_CERT_F2 *ekCert);
void freeEkCertR(EK_CERT_R *ekCert);

#endif
