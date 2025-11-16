#include "includes.h"

#ifndef DFS_H
#define DFS_H

struct edge
{
    string  originVertex;
    string  destinationVertex;
    int     distance;

    edge *next;

    edge(string originVertex, string destVertex, int distance) : originVertex(originVertex), destinationVertex(destVertex), distance(distance), next(nullptr) 
    {}
};

class AdjacencyList
{
public:
    AdjacencyList();
    ~AdjacencyList();

    void AddEdge(edge& vertexEdge);
    void RemoveEdge(string originVertex, string destinationVertex);

    int  FindIndex(string vertex) const;
    void DFS(string originVertex) const;

private:
    void InitializeList();
    void DFSRecusion(int vertexIndex, int lastIndex, vector<bool>& visited,set<pair<int, int>>& printedEdges, int& totalDistance) const;

    vector<edge*> list;
};

#endif