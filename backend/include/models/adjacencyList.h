/**
 * @file adjacencyList.h
 * @brief DFS logic
 * 
 * Preforms a DFS
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
    void dfsRecursion(int vertexIndex,int lastIndex,vector<bool>& visited,set<pair<int,int>>& printedEdges,vector<int>& distances,int currentDistance);

    vector<edge*> list;

    vector<AdjacencyNode> path;
    int                   totalDist;
};

#endif