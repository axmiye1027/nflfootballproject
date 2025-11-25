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

    void populateList(const vector<edge>& edges);

    void addEdge(edge& vertexEdge);
    void removeEdge(string originVertex, string destinationVertex);

    int  findIndex(string vertex) const;
    void dfs(string originVertex) const;

    vector<AdjacencyNode> returnPath() const;
    int                   returnDist() const;

private:
    void dfsRecusion(int vertexIndex, int lastIndex, vector<bool>& visited,set<pair<int, int>>& printedEdges, int& totalDistance) const;

    vector<edge*> list;

    vector<AdjacencyNode> path;
    int                 totalDist;
};

#endif