#!/usr/bin/python3
#coding: utf8

import sys
from copy import deepcopy
from graphs import *
from powerGraph import *

if len(sys.argv) < 5:
    print("Usage : file_graph k r file_power_graph")
    print("file_graph : a path to a file containing the graph you want to analyse.")
    print("k : the positive integer for k^G.")
    print("r : the maximum number of non zero coordinate for the vertices of k^G to generate (you can say -1 to generate the whole connected component of G)")
    print("file_power_graph : the filename under which to store the powerGraph generated.")
    exit(1)
else:
    graph = readGraphFromFile2(sys.argv[1])
    prettyPrint(graph)
    k = int(sys.argv[2])
    r = int(sys.argv[3])
    filePowerGraph = sys.argv[4]
    if r == -1:
        r = len(graph)
    powerGraph = powerN(graph,k,r)
    prettyPrint(powerGraph)
    printGraph2File2(powerGraph, filePowerGraph)
