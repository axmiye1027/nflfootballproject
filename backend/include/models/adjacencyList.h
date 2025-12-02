/**
 * @file adjacencyList.h
 * @brief DFS
 * 
 * Testings to see if it workds
 */
#include "includes.h"

#ifndef DFS_H
#define DFS_H

/**
 * @brief struct used to create an edge. It will be used for DFS
 * @param originVertex starting point
 * @param destinationVertex ending point
 * @param distance weight
 */
struct edge
{
    string  originVertex;
    string  destinationVertex;
    int     distance;

    edge *next;

    edge(string originVertex, string destVertex, int distance) : originVertex(originVertex), destinationVertex(destVertex), distance(distance), next(nullptr) 
    {}
};

/**
 * @brief 
 */
class AdjacencyList
{
public:
    AdjacencyList();
    ~AdjacencyList();

    void populateList(vector<edge>& edges);

    void addEdge(edge& vertexEdge);
    void removeEdge(string originVertex, string destinationVertex);

    int  findIndex(string vertex) const;
    void dfs(string originVertex);

    vector<AdjacencyNode> returnPath() const;
    int                   returnDist() const;

private:
    void dfsRecusion(int vertexIndex, int lastIndex, vector<bool>& visited,set<pair<int, int>>& printedEdges, int& totalDistance);

    vector<edge*> list;

    vector<AdjacencyNode> path;
    int                   totalDist;
};

#endif