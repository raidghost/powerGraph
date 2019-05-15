#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "display.h"
#include "homology.h"
#include "io.h"
#include "graphList.h"
#include "powerGraph.h"
#include "randomGraphs.h"
#include "structs.h"
#include "tools.h"


int main(int argc, char *argv[])
{
	int i;
	bool makeExp = false, ekCert = false, write2File = false, testHomo = false;
	int degreeMax = -1, supportMax = -1, verbose = 0, field = 2, n = 2, p = 2;
	char *file2Write = NULL, *tmp;
	GRAPH g;
	GRAPH graphExp;
	GRAPH_LIST* graphExpList;

	//We initiate the pseudo random generator (used in quicksort and in randomGraphs)
	srand(time(NULL));
	if(argc < 3)
	{
		fprintf(stderr, "Usage : prog [--makeExp=?] [--supportMax=?] [--degreMax=?] [--ekCert] [--write=file2Write] [--testHomo=?] [-v|-vv|-vvv] GRAPH_FILE field\nPossible values for field are : F2, R\n");
		return EXIT_FAILURE;
	}
	for(i = 1 ; i < argc - 2; i++)
	{
		if(strncmp(argv[i], "--makeExp", 9) == 0)
		{
			makeExp = true;
			strtok(argv[i], "=");
			tmp = strtok(NULL, "=");
			if(tmp && string2Int(tmp) >= 1)
				p = string2Int(tmp);
			else
			{
				fprintf(stderr, "The value of p must be >= 1. We set it to 3 by default.\n");
				p = 3;
			}
		}
		else if(strncmp(argv[i], "--supportMax", 12) == 0)
		{
			strtok(argv[i], "=");
			tmp = strtok(NULL, "=");
			if(tmp)
				supportMax = string2Int(tmp);
			else
			{
				fprintf(stderr, "The value of supportMax can be any non negative integer. If it is strictly negative, then we won't bound the support. We set it to -1 by default.\n");
				supportMax = -1;
			}
		}
		else if(strncmp(argv[i], "--degreeMax", 11) == 0)
		{
			strtok(argv[i], "=");
			tmp = strtok(NULL, "=");
			if(tmp)
				degreeMax = string2Int(tmp);
			else
			{
				fprintf(stderr, "The value of degreeMax can be any non negative integer. If it is strictly negative, then we won't bound the degree. We set it to -1 by default.\n");
				degreeMax = -1;
			}
		}

		else if(strcmp(argv[i], "--ekCert") == 0)
			ekCert = true;
		else if(strncmp(argv[i], "--write", 7) == 0)
		{
			tmp = strtok(argv[i], "=");
			tmp = strtok(NULL, "=");
			if(tmp)
			{
				file2Write = tmp;
				write2File = true;
			}
			else
				fprintf(stderr, "You must specify a file name using --write=fileName. We won't write anything on the disk.\n");
		}
		else if(strncmp(argv[i], "--testHomo", 10) == 0)
		{
			testHomo = true;
			tmp = strtok(argv[i], "=");
			tmp = strtok(NULL, "=");
			if(!tmp)
				n = -1;
			else if(string2Int(tmp) >= 0)
				n = string2Int(tmp);
			else
				n = -1;
		}
		else if(strncmp(argv[i], "-v", 2) == 0)
		{
			if(strcmp(argv[i], "-v") == 0)
				verbose = 1;
			else if(strcmp(argv[i], "-vv") == 0)
				verbose = 2;
			else
				verbose = 3;
		}
		else
			fprintf(stderr, "Sorry, I don't know the argument \"%s\".\n", argv[i]);
	}

	//We load the graph.
	if(strcmp(argv[argc - 2], "RANDOM") == 0)
		g = createRandomGraph(-1, 0.5, true);
	else if(strncmp(argv[argc - 2], "RANDOM=", 7) == 0)
	{
		tmp = strtok(argv[argc - 2], "=");
		tmp = strtok(NULL, "=");
		if(tmp)
			g = createRandomGraph(string2Int(tmp), 0.5, true);
	}
	else
	{
		g =loadGraphFromFile(argv[argc - 2]);
		if(g.mat == NULL)
		{//We cannot open the file containning the graph.
			fprintf(stderr, "Unable to read the graph from \"%s\".\n", argv[argc - 2]);
			return EXIT_FAILURE;
		}
	}

	if(strcmp(argv[argc-1], "R") == 0)
		field = 0;
	else if(strcmp(argv[argc-1], "F2") == 0)
		field = 2;
	else
	{
		fprintf(stderr, "The field should be either \"F2\" or \"R\". We use F2 by default.\n");
		field = 2;
	}


	if(verbose >= 3)
		displayGraph(&g);

	if(makeExp)
	{
		graphExpList = genPowerGraph(&g, p, supportMax, degreeMax);
		graphExp = graphList2Mat(graphExpList);
		if(verbose >= 3)
		{
			displayGraphList(graphExpList);
			displayGraph(&graphExp);
		}
		if(write2File)
			writeGraph2File(&graphExp, file2Write);
	}

	if(ekCert)
	{
		if(!makeExp)
			fprintf(stderr, "We can only look for an edge clique certificate in a power graph. Use \"--makeExp\".\n");
		else
		{
			if(field == 2)
			{
				EK_CERT_F2 ekCertificate;
				ekCertificate = findEkCertF2(&g, graphExpList, p);
				if(ekCertificate.weight)
				{//If we have found an edge clique certificate.
					printf("We found an edge clique certificate !\n");
					if(verbose >= 1)
						displayEkCertF2(&ekCertificate, false);
					else
						printf("Use -v to display it.\n");
				}
				else
				{
					printf("We could not find any edge clique certificate.\n");
					if(supportMax >= 0)
						printf("Perhaps you should increase the support (\"--supportMax\").\n");
				}
				freeEkCertF2(&ekCertificate);
			}
			else
			{
				EK_CERT_R ekCertificate;
				ekCertificate = findEkCertR(&g, graphExpList, p, verbose);
				if(ekCertificate.weight)
				{//If we have found an edge clique certificate.
					printf("We found an edge clique certificate !\n");
					if(verbose >= 1)
						displayEkCertR(&ekCertificate, false);
					else
						printf("Use -v to display it.\n");
				}
				else
				{
					printf("We could not find any edge clique certificate.\n");
					if(supportMax >= 0)
						printf("Perhaps you should increase the support (\"--supportMax\").\n");
				}
				freeEkCertR(&ekCertificate);
			}
		}
	}

	if(testHomo)
	{
		if(makeExp)
		//We run the test on the power graph.
			printf("Hn(%d^G) is true until n = %d (n_max = %d)\n", p, testHn(&graphExp, n, field, verbose), n);
		else
		//We run the test on the graph itself.
			printf("Hn(G) is true until n = %d (n_max = %d)\n", testHn(&g, n, field, verbose), n);
	}

	//Finally, we free useless memory.	
	if(makeExp)
	{
		freeGraphList(graphExpList);
		freeGraph(&graphExp);
	}
	freeGraph(&g);
	return EXIT_SUCCESS;
}
