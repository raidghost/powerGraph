#ifndef CHECK
#define CHECK

#include "structs.h"

unsigned int nbEdges(GRAPH *g);
int commonDegree(GRAPH *g);
GRAPH_LIST* checkEkCertF2(GRAPH_LIST* powerGraph, EK_CERT_F2* ekCert);
GRAPH_LIST* checkEkCertR(GRAPH_LIST* powerGraph, EK_CERT_R* ekCert);

#endif
