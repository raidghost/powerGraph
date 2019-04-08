#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "display.h"
#include "homology.h"
#include "io.h"
#include "graphList.h"
#include "powerGraph.h"
#include "structs.h"
#include "tools.h"


int main(int argc, char *argv[])
{
	bool makeExp = false, save2File = false;
	int argsOffset = 0, supportMax = -1, verbose = 0, field = 2, n = 7, p = 3;
	char *file2Save, *tmp;
	GRAPH g;

	//We initiate the pseudo random generator (used in quicksort)
	srand(time(NULL));
	if(argc < 5)
	{
		fprintf(stderr, "Usage : prog [--makeExp[=outputFile] [--supportMax=k] p] GRAPH_FILE n field [-v]\nPossible values for field are : F2, R\n");
		return EXIT_FAILURE;
	}
	if(strncmp(argv[1], "--makeExp", 9) == 0)
	{//In this case, we must generate the powerGraph and work on it instead of the graph itself.
		makeExp = true;
		strtok(argv[1], "=");
		file2Save = strtok(NULL, "=");
		if(file2Save)
		//We are in the case --makeExp=file2Save
			save2File = true;
		argsOffset++;
		if(strncmp(argv[2], "--supportMax=", 13) == 0)
		{//If the support must be bounded
			strtok(argv[2], "=");
			tmp = strtok(NULL, "=");
			if(tmp)
				supportMax = string2Int(tmp);
			argsOffset++;
		}
		p = string2Int(argv[1 + argsOffset]);
		argsOffset++;
		if(p <= 0)
		{
			fprintf(stderr, "The value \"p\" must be a positive integer. Beware that in case that \"p\" is not prime, lines in the power graph are not edge-cliques !\n");
			exit(EXIT_FAILURE);
		}
	}

	g = loadGraphFromFile(argv[1 + argsOffset]);
	if(g.mat == NULL)
	{//We cannot open the file containning the graph.
		fprintf(stderr, "Unable to read the graph from \"%s\".\n", argv[1 + argsOffset]);
		return EXIT_FAILURE;
	}
	n = string2Int(argv[2 + argsOffset]);

	if(n <= 1)
	{
		fprintf(stderr, "There is no point in testing Hn for n <= 1.\nWe take n=2 by default.\n");
		n = 2;
	}

	//Verbosity.
	if(argc >= 5)
	{
		if(strcmp(argv[4 + argsOffset], "-v") == 0)
			verbose = 1;
		else if(strcmp(argv[4 + argsOffset], "-vv") == 0)
			verbose = 2;
		else if(strcmp(argv[4 + argsOffset], "-vvv") == 0)
			verbose = 3;
	}

	if(strcmp(argv[3 + argsOffset], "F2") == 0)
		field = 2;
	else if(strcmp(argv[3 + argsOffset], "R") == 0)
		field = 0;
	else
	{
		fprintf(stdout, "Possible values for field are : F2, R\nWe use R by default.");
		field = 0;
	}

	if(verbose >= 3)
		displayGraph(&g);

	if(makeExp)
	{
		GRAPH graphExp;
		GRAPH_LIST* graphExpList;
		EK_CERT ekCertificate;
		graphExpList = genPowerGraph(&g, p, supportMax);
		if(verbose >= 3)
			displayGraphList(graphExpList);
		graphExp = graphList2Mat(graphExpList);
		ekCertificate = findEkCert(&g, graphExpList, p, field);
		if(ekCertificate.weight)
		{//If we have found an edge clique certificate.
			printf("We found an edge clique certificate !\n");
			displayEkCert(&ekCertificate, false);
		}
		else
			printf("We could not find any certificate...\n");
		freeEkCert(&ekCertificate);
		freeGraphList(graphExpList);
		if(save2File)
			writeGraph2File(&graphExp, file2Save);
		//printf("Hn(%d^G) is true until n = %d\n (n_max = %d)", p, testHn(&graphExp, n, field, verbose), n);
		freeGraph(&graphExp);
	}
	else
	{
		if(verbose >= 3)
			displayGraph(&g);
		printf("Hn(G) est vraie jusqu'à n = %d\n (n_max = %d)", testHn(&g, n, field, verbose), n);
	}
	freeGraph(&g);
	return EXIT_SUCCESS;
}
