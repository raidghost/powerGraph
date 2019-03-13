#!/usr/bin/python3
#coding: utf8

def readGraphFromFile(fileName):
    """Read a graph specification from a text file and load it into a dictionary."""
    graph = {}
    f = open(fileName, "r")
    graphFromFile = f.readlines()
    f.close()
    if graphFromFile[0] != "#vertices\n":
        print("The file is not properly formated.")
        exit(1)
    else:
        del(graphFromFile[0])
        #We add the vertices
        for line in graphFromFile:
            if line == "#edges\n":
                break
            elif not line.isspace():
                vertex = line[:line.find("\n")]
                graph[vertex] = []

        graphFromFile = graphFromFile[graphFromFile.index("#edges\n")+1:]
        #We add the edges
        for line in graphFromFile:
            if not line.isspace():
                line = line[:line.find("\n")]
                array = line.split("-")
                u = array[0]
                for v in array[1:]:
                    if v != u and v not in graph[u]:
                        graph[u].append(v)
                        graph[v].append(u)
                    u = v
        return graph

def readGraphFromFile2(fileName):
    """Read a graph specification from a text file in a simpler format."""
    graph = {}
    f = open(fileName, "r")
    graphFromFile = f.readlines()
    f.close()
    nbVertices = int(graphFromFile[0])
    del(graphFromFile[0])
    for line in graphFromFile:
        line = line[:line.find("\n")]
        array = line.split("-")
        u = int(array[0]) + 1
        v = int(array[1]) + 1
        if u not in graph:
            graph[u] = []
        if v  not in graph:
            graph[v] = []
        if v != u:
            if v not in graph[u]:
                graph[u].append(v)
            if u not in graph[v]:
                graph[v].append(u)
    return graph

def printGraph2File2(graph, fileName):
    """Print a graph to a file using the simpler format."""
    f = open(fileName, "w")
    nbVertices = len(graph.keys())
    vertices = list(graph.keys())
    vertices.sort()
    verticesInt = range(nbVertices)
    f.write(str(nbVertices) + "\n")
    for u in vertices:
        indexU = vertices.index(u)
        for v in graph[u]:
            indexV = vertices.index(v)
            if indexV > indexU:
                f.write("{}-{}\n".format(indexU,indexV))
    f.close()

def graph2Integer(graph):
    """Cast every vertices of a graph into integer."""
    graphInt = {}
    for u in graph.keys():
        try:
            if int(u) not in graphInt.keys():
                graphInt[int(u)] = []
                for i in range(len(graph[u])):
                    graphInt[int(u)].append(int(graph[u][i]))
                #To make the graph easier to read
                graphInt[int(u)].sort()
            else:
                print("Warning : there is a vertex {} but also a vertex {}. Please rename it.".format(u,int(u)))
        except:
            print("The graph is not properply formated.")
            exit(1)
    return graphInt

def orderGraph(graph):
    """Given an integer graph, order the lists of neighbours."""
    for u in graph:
        try:
            graph[u].sort()
        except:
            print("The vertices of the graph should be integers.")
            exit(1)

def fusionGraphs(g1,g2):
    """Given two graphs g1 and g2, add g2 as a subgraph of g1."""
    for u in g2:
        if u not in g1:
            g1[u] = []
        for v in g2[u]:
            if v not in g1[u]:
                g1[u].append(v)
    orderGraph(g1)

def incidenceMatrix(graph):
    """Given an integer graph as a dictionary, computes an oriented incidence matrix"""
    nbVertices = len(graph)
    columns = []
    for i in graph:
        for j in graph[i]:
            try:
                column = nbVertices * [0]
                column[i-1] = -1
                column[j-1] = 1
                columns.append(column)
                #In order not to count twice the same edge
                graph[j].remove(i)
            except:
                print("Please index the vertices using all integers from 1 to n.")
                exit(1)
    m1 = np.matrix(columns)
    return m1.transpose()

def prettyPrint(graph):
    for u in graph:
        print("{} ->".format(u))
        for v in graph[u]:
            print("\t{}".format(v))

def graphTuple2String(graph):
    """Given a graph whose vertices are tuple, this function converts it to a
    graph whose vertices are string."""
    graphString = {}
    for u in graph:
        w = ''
        lengthU = len(u)
        for i in range(lengthU):
            w += str(u[i])
        graphString[w] = []
        for v in graph[u]:
            x = ''
            lengthV = len(v)
            for i in range(lengthV):
                x += str(v[i])
            graphString[w].append(x)
    return graphString
