#!/usr/bin/python3
#coding: utf8

from copy import deepcopy
from graphs import *

def powerN(graph,n,k):
    """Given a graph G, compute the connected component of n^G containing G with
    support bounded by k."""
    nbVertices = len(graph)
    if k == 1:
        powerGraph = {}
        for i in range(nbVertices):
            if i+1 not in graph:
                print("The graph must contain all integers from 1 to nbVertices.")
                exit(1)
            u = i * (0,) + (1,) + (nbVertices - 1 - i) * (0,)
            powerGraph[u] = []
        for i in range(nbVertices):
            u = i * (0,) + (1,) + (nbVertices - 1 - i) * (0,)
            for j in graph[i+1]:
                if j > i:
                    v = (j-1) * (0,) + (1,) + (nbVertices - j) * (0,)
                    powerGraph[u].append(v) 
                    powerGraph[v].append(u)
    else:
        powerGraphOld = powerN(graph,n,k-1)
        powerGraph = deepcopy(powerGraphOld)
        for w in powerGraphOld:
            if len([i for i in w if i != 0]) == k-1:
            #If the support of w is exactly k-1
                for i in range(nbVertices):
                    if w[i] != 0:
                    #For all vertex u of G such that "u is in the support of w"
                        u = i * (0,) + (1,) + (nbVertices -1 -i) * (0,)
                        for j in graph[i+1]:
                            if w[j-1] == 0:
                            #For all v such that uv in E but v is not in support of w"
                                v = (j-1) * (0,) + (1,) + (nbVertices -j) * (0,)
                                direction = ()
                                for l in range(nbVertices):
                                    direction += (v[l] - u[l],)
                                newVertices = n * [()]
                                newVertices[0] = w
                                for m in range(1,n):
                                    for l in range(nbVertices):
                                        newVertices[m] += ((newVertices[m-1][l] + direction[l]) %n,)
                                    if newVertices[m] not in powerGraph:
                                        powerGraph[newVertices[m]] = []
                                for m in range(n):
                                    for p in range(m+1,n):
                                        if newVertices[m] not in powerGraph[newVertices[p]]:
                                            powerGraph[newVertices[p]].append(newVertices[m])
                                        if newVertices[p] not in powerGraph[newVertices[m]]:
                                            powerGraph[newVertices[m]].append(newVertices[p])
                                            
    orderGraph(powerGraph)
    return powerGraph
